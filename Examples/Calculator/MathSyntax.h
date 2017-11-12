#ifndef MathSyntaxH
#define MathSyntaxH

#include "Syntax.h"

struct MathSyntax
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
};

#endif
