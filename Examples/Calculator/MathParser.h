#ifndef MathParserH
#define MathParserH

#include <map>
#include <string>
#include <iostream>
#include <iomanip> // ...

#include "Parser.h"
#include "MathFunctions.h"

class MathParser
{
    Syntax mathExpression = !assignment & additiveExpression & whitespaces & (Syntax::endOfFile() | Syntax::error("Unexpected character"));
    Syntax assignment = whitespaces & (constantAssignment | (variable & assignmentOperator));
    Syntax constantAssignment = ~(constant & assignmentOperator & Syntax::error("Cannot assign to constant"));
    Syntax assignmentOperator = ~(whitespaces & Syntax::sequence("="));
    Syntax additiveExpression = multiplicativeExpression & !(whitespaces & ~Syntax::oneOf("+-") & (additiveExpression | Syntax::error("Factor expected")));
    Syntax multiplicativeExpression = exponentalExpression & !(whitespaces & ~Syntax::oneOf("*/") & (multiplicativeExpression | Syntax::error("Factor expected")));
    Syntax exponentalExpression = unaryOperation & !(whitespaces & Syntax::sequence("^") & (exponentalExpression | Syntax::error("Factor expected")));
    Syntax unaryOperation = factor | (whitespaces & ~Syntax::oneOf("-+") & unaryOperation) | Syntax::error("Factor expected");
    Syntax factor = whitespaces & (bracedExpression | functionCall | variable | realNumber);
    Syntax bracedExpression = Syntax::sequence("(") & additiveExpression & whitespaces & (Syntax::sequence(")") | Syntax::error("Closing bracket expected"));
    Syntax functionCall = ~identifier & whitespaces & Syntax::sequence("(") & argumentList & whitespaces & (Syntax::sequence(")") | Syntax::error("Closing bracket expected"));
    Syntax argumentList = ~!(additiveExpression & *(whitespaces & Syntax::sequence(",") & additiveExpression));
    Syntax constant = ~(Syntax::sequence("ans") | Syntax::sequence("pi") | Syntax::sequence("e"));
    Syntax variable = identifier;
    Syntax identifier = ~(letter & *(letter | digit));
    Syntax realNumber = +digit & !(Syntax::sequence(".") & +digit) & !(Syntax::oneOf("Ee") & !Syntax::oneOf("+-") & +digit);
    Syntax whitespaces = ~*Syntax::oneOf(" \n\r\t\f\v");
    Syntax letter = ~(Syntax::range('a', 'z') | Syntax::range('A', 'Z'));
    Syntax digit = ~Syntax::range('0', '9');

    std::size_t objectId;
    ObjectTree objectTree;
    MathFunctions mathFunctions;
	std::map<std::string, double> variables;
    const std::string *expression;
    std::string errorString;

    bool evaluateMathExpression()
    {
        objectId++;

        double result = 0;
        double *resultPointer = &result;
        if (assignment == objectTree[objectId].syntaxId)
            resultPointer = evaluateAssignment();

        if (!evaluateAdditiveExpression(*resultPointer))
            return false;

        variables["ans"] = *resultPointer;
        return true;
    }

    double *evaluateAssignment()
    {
        objectId++;

        const Object &variable = objectTree[objectId++];
        std::string variableName(*expression, variable.from, variable.to - variable.from);
        return &variables[variableName];
    }

    bool evaluateAdditiveExpression(double &result)
    {
        size_t parent = objectId++;

        if (!evaluateMultiplicativeExpression(result))
            return false;

        if (objectId >= objectTree.size())
            return true;

        const Object &operation = objectTree[objectId];
        if (operation.parent != parent)
            return true;

        objectId++;
        double secondArgument;
        if (!evaluateAdditiveExpression(secondArgument))
            return false;

        if ((*expression)[operation.from] == '+')
            result += secondArgument;
        else
            result -= secondArgument;

        return true;
    }

    bool evaluateMultiplicativeExpression(double &result)
    {
        size_t parent = objectId++;

        if (!evaluateExponentalExpression(result))
            return false;

        if (objectId >= objectTree.size())
            return true;

        const Object &operation = objectTree[objectId];
        if (operation.parent != parent)
            return true;

        objectId++;
        double secondArgument;
        if (!evaluateMultiplicativeExpression(secondArgument))
            return false;

        if ((*expression)[operation.from] == '*')
            result *= secondArgument;
        else
            result /= secondArgument;

        return true;
    }

    bool evaluateExponentalExpression(double &result)
    {
        size_t parent = objectId++;

        if (!evaluateUnaryOperation(result))
            return false;

        if (objectId >= objectTree.size())
            return true;

        if (objectTree[objectId].parent != parent)
            return true;

        double secondArgument;
        if (!evaluateExponentalExpression(secondArgument))
            return false;

        if (!mathFunctions.execute("pow", { result, secondArgument })) {
            errorString = mathFunctions.getErrorString();
            return false;
        }

        result = mathFunctions.getResult();
        return true;
    }

    bool evaluateUnaryOperation(double &result)
    {
        objectId++;

        const Object &object = objectTree[objectId];
        if (factor == object.syntaxId)
            return evaluateFactor(result);

        objectId++;
        if (!evaluateUnaryOperation(result))
            return false;

        if ((*expression)[object.from] == '-')
            result = -result;

        return true;
    }

    bool evaluateFactor(double &result)
    {
        objectId++;

        const Object &object = objectTree[objectId];

        if (bracedExpression == object.syntaxId)
            return evaluateBracedExpression(result);

        if (functionCall == object.syntaxId)
            return evaluateFunctionCall(result);

        if (variable == object.syntaxId)
            return evaluateVariable(result);

        return evaluateRealNumber(result);
    }

    bool evaluateBracedExpression(double &result)
    {
        objectId++;

        return evaluateAdditiveExpression(result);
    }

    bool evaluateFunctionCall(double &result)
    {
        size_t parent = objectId++;

        const Object &function = objectTree[objectId++];
        std::string functionName(*expression, function.from, function.to - function.from);

        std::vector<double> params;
        params.reserve(5);

        while (objectId < objectTree.size()) {
            if (objectTree[objectId].parent != parent)
                break;

            params.push_back(0);
            if (!evaluateAdditiveExpression(params.back()))
                return false;
        }

        if (!mathFunctions.execute(functionName, params)) {
            errorString = mathFunctions.getErrorString();
            return false;
        }

        result = mathFunctions.getResult();
        return true;
    }

    bool evaluateVariable(double &result)
    {
        const Object &variable = objectTree[objectId++];
        std::string variableName(*expression, variable.from, variable.to - variable.from);

        if (variables.find(variableName) == variables.end()) {
            errorString = std::string("Unknown variable '") + variableName + "'";
            return false;
        }

        result = variables[variableName];
        return true;
    }

    bool evaluateRealNumber(double &result)
    {
        const Object &realNumber = objectTree[objectId++];
        std::string valueStr(*expression, realNumber.from, realNumber.to - realNumber.from);
        result = std::stod(valueStr.c_str());
        return true;
    }


    public:

        MathParser()
        {
			variables["e"] = 2.7182818284590452;
			variables["pi"] = 3.1415926535897932;
			variables["ans"] = 0;
        }

        double getResult() const
        {
            return variables.find("ans")->second;
        }

        bool solve(const std::string &expression)
        {
            objectId = 0;
            objectTree.clear();
            this->expression = &expression;

            StringSequencer stringSequencer(expression);
            Parser parser(mathExpression, stringSequencer, objectTree);

            errorString.clear();
            if (!parser.parse()) {
                errorString = parser.getErrorString();
                return false;
            }

            /*std::string str[250];
            str[mathExpression.getData().id] = "mathExpression";
            str[assignment.getData().id] = "assignment";
            str[constantAssignment.getData().id] = "constantAssignment";
            str[assignmentOperator.getData().id] = "assignmentOperator";
            str[additiveExpression.getData().id] = "additiveExpression";
            str[multiplicativeExpression.getData().id] = "multiplicativeExpression";
            str[exponentalExpression.getData().id] = "exponentalExpression";
            str[unaryOperation.getData().id] = "unaryOperation";
            str[factor.getData().id] = "factor";
            str[bracedExpression.getData().id] = "bracedExpression";
            str[functionCall.getData().id] = "functionCall";
            str[argumentList.getData().id] = "argumentList";
            str[constant.getData().id] = "constant";
            str[variable.getData().id] = "variable";
            str[identifier.getData().id] = "identifier";
            str[realNumber.getData().id] = "realNumber";
            str[whitespaces.getData().id] = "whitespaces";
            str[letter.getData().id] = "letter";
            str[digit.getData().id] = "digit";

            for (std::size_t objectId = 0; objectId < objectTree.size(); objectId++) {

                int depth = 0;
                for (std::size_t parent = objectTree[objectId].parent; parent != std::size_t(-1); parent = objectTree[parent].parent)
                    depth++;

                std::cout << std::setfill('0') << std::setw(4) << objectId;
                std::cout << ": " << std::string(depth, ' ');

                if(!str[objectTree[objectId].syntaxId].empty())
                    std::cout << str[objectTree[objectId].syntaxId].c_str();
                std::cout << "[" << objectTree[objectId].syntaxId << "]";

                std::cout << ", '" << std::string(&expression.c_str()[objectTree[objectId].from], &expression.c_str()[objectTree[objectId].to]) << "'";
                std::cout << std::endl;
            }*/

            return evaluateMathExpression();
        }

		std::string getErrorString() const
		{
			return errorString;
		}
};

#endif
