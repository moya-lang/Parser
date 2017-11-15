#ifndef MathParserH
#define MathParserH

#include <map>
#include <string>

#include "MathSyntax.h"
#include "MathFunctions.h"
#include "Parser.h"

class MathParser
{
    const std::string &expression;

    MathSyntax mathSyntax;
    MathFunctions mathFunctions;
    Moya::ObjectTree objectTree;

    std::map<std::string, double> variables;
    std::string errorString;

    bool evaluateMathExpression()
    {
        objectTree.next();

        double result = 0;
        double *resultPointer = &result;
        if (mathSyntax.assignment == objectTree->syntaxId)
            resultPointer = evaluateAssignment();

        if (!evaluateAdditiveExpression(*resultPointer))
            return false;

        variables["ans"] = *resultPointer;
        return true;
    }

    double *evaluateAssignment()
    {
        objectTree.next();

        const Moya::Object &variable = *objectTree;
        std::string variableName(expression, variable.from, variable.to - variable.from);

        objectTree.next();

        return &variables[variableName];
    }

    bool evaluateAdditiveExpression(double &result)
    {
        size_t parent = objectTree.next();

        if (!evaluateMultiplicativeExpression(result))
            return false;

        if (!objectTree.hasNext())
            return true;

        const Moya::Object &operation = *objectTree;
        if (operation.parent != parent)
            return true;

        objectTree.next();

        double secondArgument;
        if (!evaluateAdditiveExpression(secondArgument))
            return false;

        if (expression[operation.from] == '+')
            result += secondArgument;
        else
            result -= secondArgument;

        return true;
    }

    bool evaluateMultiplicativeExpression(double &result)
    {
        size_t parent = objectTree.next();

        if (!evaluateExponentalExpression(result))
            return false;

        if (!objectTree.hasNext())
            return true;

        const Moya::Object &operation = *objectTree;
        if (operation.parent != parent)
            return true;

        objectTree.next();

        double secondArgument;
        if (!evaluateMultiplicativeExpression(secondArgument))
            return false;

        if (expression[operation.from] == '*')
            result *= secondArgument;
        else
            result /= secondArgument;

        return true;
    }

    bool evaluateExponentalExpression(double &result)
    {
        size_t parent = objectTree.next();

        if (!evaluateUnaryOperation(result))
            return false;

        if (!objectTree.hasNext())
            return true;

        if (objectTree->parent != parent)
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
        objectTree.next();

        const Moya::Object &object = *objectTree;
        if (mathSyntax.factor == object.syntaxId)
            return evaluateFactor(result);

        objectTree.next();

        if (!evaluateUnaryOperation(result))
            return false;

        if (expression[object.from] == '-')
            result = -result;

        return true;
    }

    bool evaluateFactor(double &result)
    {
        objectTree.next();

        const Moya::Object &object = *objectTree;

        if (mathSyntax.bracedExpression == object.syntaxId)
            return evaluateBracedExpression(result);

        if (mathSyntax.functionCall == object.syntaxId)
            return evaluateFunctionCall(result);

        if (mathSyntax.variable == object.syntaxId)
            return evaluateVariable(result);

        return evaluateRealNumber(result);
    }

    bool evaluateBracedExpression(double &result)
    {
        objectTree.next();

        return evaluateAdditiveExpression(result);
    }

    bool evaluateFunctionCall(double &result)
    {
        size_t parent = objectTree.next();

        const Moya::Object &function = *objectTree;
        std::string functionName(expression, function.from, function.to - function.from);

        objectTree.next();

        std::vector<double> params;
        params.reserve(5);

        while (objectTree.hasNext()) {
            if (objectTree->parent != parent)
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
        const Moya::Object &variable = *objectTree;
        std::string variableName(expression, variable.from, variable.to - variable.from);

        objectTree.next();

        if (variables.find(variableName) == variables.end()) {
            errorString = std::string("Unknown variable '") + variableName + "'";
            return false;
        }

        result = variables[variableName];
        return true;
    }

    bool evaluateRealNumber(double &result)
    {
        const Moya::Object &realNumber = *objectTree;
        std::string valueStr(expression, realNumber.from, realNumber.to - realNumber.from);

        objectTree.next();

        result = std::stod(valueStr.c_str());
        return true;
    }


    public:

        MathParser(const std::string &expression) :
            expression(expression)
        {
            variables["e"] = 2.7182818284590452;
            variables["pi"] = 3.1415926535897932;
            variables["ans"] = 0;
        }

        double getResult() const
        {
            return variables.find("ans")->second;
        }

        bool solve()
        {
            objectTree.reset();
            errorString.clear();

            Moya::StringSequencer stringSequencer(expression);
            Moya::Parser parser(mathSyntax.mathExpression, stringSequencer, objectTree);

            if (!parser.parse()) {
                errorString = parser.getErrorString();
                return false;
            }

            return evaluateMathExpression();
        }

        std::string getErrorString() const
        {
            return errorString;
        }
};

#endif
