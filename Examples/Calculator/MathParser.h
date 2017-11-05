#ifndef MathParserH
#define MathParserH

#include <map>
#include <string>

#include "Parser.h"
#include "MathFunctions.h"

class MathParser
{
    Syntax mathExpression = !assignment & additiveExpression & whitespaces & (Syntax::endOfFile() | Syntax::error("Unexpected character"));
    Syntax assignment = whitespaces & variableWrite & (whitespaces & Syntax::sequence("="));
    Syntax additiveExpression = multiplicativeExpression & !(whitespaces & ~Syntax::oneOf("+-") & additiveExpression);
    Syntax multiplicativeExpression = exponentalExpression & !(whitespaces & ~Syntax::oneOf("*/") & multiplicativeExpression);
    Syntax exponentalExpression = unaryOperation & !(whitespaces & Syntax::sequence("^") & exponentalExpression);
    Syntax unaryOperation = factor | (~Syntax::oneOf("-+") & unaryOperation);
    Syntax factor = (whitespaces & (bracedExpression | functionCall | variableRead | realNumber)) | Syntax::error("Factor expected");
    Syntax bracedExpression = Syntax::sequence("(") & additiveExpression & whitespaces & (Syntax::sequence(")") | Syntax::error("Closing bracket expected"));
    Syntax functionCall = identifier & whitespaces & Syntax::sequence("(") & argumentList & whitespaces & (Syntax::sequence(")") | Syntax::error("Closing bracket expected"));
    Syntax argumentList = !(additiveExpression & *(whitespaces & Syntax::sequence(",") & (additiveExpression | Syntax::error("Function argument expected"))));
    Syntax variableWrite = ((Syntax::sequence("pi") | Syntax::sequence("e")) & Syntax::error("Cannot assign to constant")) | identifier;
    Syntax variableRead = identifier;
    Syntax identifier = (letter & *(letter | digit)) | Syntax::error("Identifier expected");
    Syntax realNumber = (integerNumber & !(Syntax::sequence(".") & integerNumber) & !(Syntax::oneOf("Ee") & !Syntax::oneOf("+-") & integerNumber)) | Syntax::error("Real number expected");
    Syntax integerNumber = +digit | Syntax::error("Integer number expected");
    Syntax whitespaces = *Syntax::oneOf(" \n\r\t\f\v");
    Syntax letter = Syntax::range('a', 'z') | Syntax::range('A', 'Z') | Syntax::error("Alphabetic character expected");
    Syntax digit = Syntax::range('0', '9') | Syntax::error("Decimal digit expected");

	std::string errorString;
	MathFunctions mathFunctions;
	std::map<std::string, double> variables;


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
            ObjectTree objectTree;
            StringSequencer stringSequencer(expression);
            Parser parser(mathExpression, stringSequencer, objectTree);

            if (!parser.parse()) {
                errorString = parser.getErrorString();
                return false;
            }

            std::string str[250];
            str[mathExpression.getData().id] = "mathExpression";
            str[assignment.getData().id] = "assignment";
            str[additiveExpression.getData().id] = "additiveExpression";
            str[multiplicativeExpression.getData().id] = "multiplicativeExpression";
            str[unaryOperation.getData().id] = "unaryOperation";
            str[factor.getData().id] = "factor";
            str[bracedExpression.getData().id] = "bracedExpression";
            str[functionCall.getData().id] = "functionCall";
            str[argumentList.getData().id] = "argumentList";
            str[variableWrite.getData().id] = "variableWrite";
            str[variableRead.getData().id] = "variableRead";
            str[identifier.getData().id] = "identifier";
            str[realNumber.getData().id] = "realNumber";
            str[integerNumber.getData().id] = "integerNumber";
            str[whitespaces.getData().id] = "whitespaces";
            str[letter.getData().id] = "letter";
            str[digit.getData().id] = "digit";

            for (std::size_t objectId = 0; objectId < objectTree.size(); objectId++) {
                std::cout << "objectId: " << objectId;
                std::cout << ", parentId: " << objectTree[objectId].parent;
                std::cout << ", syntaxId: " << objectTree[objectId].syntaxId;
                if (!str[objectTree[objectId].syntaxId].empty())
                    std::cout << " (" << str[objectTree[objectId].syntaxId] << ")";
                std::cout << ", content: '" << std::string(&expression.c_str()[objectTree[objectId].from], &expression.c_str()[objectTree[objectId].to]) << "'";
                std::cout << std::endl;
            }

            return true;
        }

		std::string getErrorString() const
		{
			return errorString;
		}
};

#endif
