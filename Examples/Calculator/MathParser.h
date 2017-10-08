#ifndef MathParserH
#define MathParserH

#include "Parser.h"

class MathParser : public Parser
{
    Syntax mathExpression = !assignment & additiveExpression & ~!whitespaces;
    Syntax assignment = ~!whitespaces & variable & ~!whitespaces & ~Syntax('=');
    Syntax additiveExpression = multiplicativeExpression & !(~!whitespaces & (~Syntax('+') | ~Syntax('-')) & additiveExpression);
    Syntax multiplicativeExpression = exponentalExpression & !(~!whitespaces & (~Syntax('*') | ~Syntax('/')) & multiplicativeExpression);
    Syntax exponentalExpression = unaryOperation & !(~!whitespaces & Syntax('^') & exponentalExpression);
    Syntax unaryOperation = factor | ((~Syntax('+') | ~Syntax('-')) & unaryOperation);
    Syntax factor = (~!whitespaces & (bracedExpression | functionCall | variable | realNumber)) | Syntax::error("Factor expected");
    Syntax bracedExpression = ~Syntax('(') & additiveExpression & ~!whitespaces & (~Syntax(')') | Syntax::error("Closing bracket expected"));
    Syntax functionCall = identifier & ~!whitespaces & ~Syntax('(') & argumentList & ~!whitespaces & (~Syntax(')') | Syntax::error("Closing bracket expected"));
    Syntax argumentList = !(additiveExpression & *(~!whitespaces & ~Syntax(',') & (additiveExpression | Syntax::error("Function argument expected"))));
    Syntax variable = ~identifier;
    Syntax identifier = ~(letter & *(letter | digit)) | Syntax::error("Identifier expected");
    Syntax realNumber = ~(integerNumber & !(Syntax('.') & integerNumber) &
        !((Syntax('E') | Syntax('e')) & !(Syntax('+') | Syntax('-')) & integerNumber)) | Syntax::error("Real number expected");
    Syntax integerNumber = ~+digit | Syntax::error("Integer number expected");
    Syntax whitespaces = ~+(Syntax(' ') | Syntax('\n') | Syntax('\r') | Syntax('\t') | Syntax('\f') | Syntax('\v')) | Syntax::error("Whitespace expected");
    Syntax letter = ~((Syntax('a') - Syntax('z')) | (Syntax('A') - Syntax('Z'))) | Syntax::error("Alphabetic character expected");
    Syntax digit = ~(Syntax('0') - Syntax('9')) | Syntax::error("Decimal digit expected");


    public:

        MathParser() : Parser(mathExpression)
        {
        }
};

#endif
