#ifndef MathParserH
#define MathParserH

#include <map>
#include <string>

#include "Parser.h"
#include "MathFunctions.h"

class MathParser
{
    Syntax mathExpression = !assignment & additiveExpression & ~!whitespaces;
    Syntax assignment = ~!whitespaces & variableWrite & ~(!whitespaces & Syntax('='));
    Syntax additiveExpression = multiplicativeExpression & !(~!whitespaces & (~Syntax('+') | ~Syntax('-')) & additiveExpression);
    Syntax multiplicativeExpression = exponentalExpression & !(~!whitespaces & (~Syntax('*') | ~Syntax('/')) & multiplicativeExpression);
    Syntax exponentalExpression = unaryOperation & !(~!whitespaces & Syntax('^') & exponentalExpression);
    Syntax unaryOperation = factor | ((~Syntax('+') | ~Syntax('-')) & unaryOperation);
    Syntax factor = (~!whitespaces & (bracedExpression | functionCall | variableRead | realNumber)) | Syntax::error("Factor expected");
    Syntax bracedExpression = ~Syntax('(') & additiveExpression & ~!whitespaces & (~Syntax(')') | Syntax::error("Closing bracket expected"));
    Syntax functionCall = identifier & ~!whitespaces & ~Syntax('(') & argumentList & ~!whitespaces & (~Syntax(')') | Syntax::error("Closing bracket expected"));
    Syntax argumentList = !(additiveExpression & *(~!whitespaces & ~Syntax(',') & (additiveExpression | Syntax::error("Function argument expected"))));
    Syntax variableWrite = ~(((Syntax('pi') | Syntax('e')) & Syntax::error("Cannot assign to constant")) | identifier);
    Syntax variableRead = ~identifier;
    Syntax identifier = ~(letter & *(letter | digit)) | Syntax::error("Identifier expected");
    Syntax realNumber = ~(integerNumber & !(Syntax('.') & integerNumber) & !((Syntax('E') | Syntax('e')) & !(Syntax('+') | Syntax('-')) & integerNumber)) | Syntax::error("Real number expected");
    Syntax integerNumber = ~+digit | Syntax::error("Integer number expected");
    Syntax whitespaces = ~+(Syntax(' ') | Syntax('\n') | Syntax('\r') | Syntax('\t') | Syntax('\f') | Syntax('\v')) | Syntax::error("Whitespace expected");
    Syntax letter = ~((Syntax('a') - Syntax('z')) | (Syntax('A') - Syntax('Z'))) | Syntax::error("Alphabetic character expected");
    Syntax digit = ~(Syntax('0') - Syntax('9')) | Syntax::error("Decimal digit expected");

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
            for (std::size_t objectId = 0; objectId < objectTree.size(); objectId++)
                std::cout << "ID: " << objectId << ", " << str[objectTree[objectId].syntaxId] << std::endl;

            // TODO:
            // - Use ~ operator in oposite way: By default all are silent and ~ is not.
            // - Use const char * instead of size_t position in parsed object to handle hidden objects
            // - Replace strings with const char * if needed
            // - convert range to double-character content
            // ...

            return true;
        }

		std::string getErrorString() const
		{
			return errorString;
		}
};

#endif
