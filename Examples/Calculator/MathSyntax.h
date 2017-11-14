#ifndef MathSyntaxH
#define MathSyntaxH

#include "Syntax.h"

struct MathSyntax
{
    Moya::Syntax mathExpression = !assignment & additiveExpression & whitespaces & (Moya::Syntax::endOfFile() | Moya::Syntax::error("Unexpected character"));
    Moya::Syntax assignment = whitespaces & (constantAssignment | (variable & assignmentOperator));
    Moya::Syntax constantAssignment = ~(constant & assignmentOperator & Moya::Syntax::error("Cannot assign to constant"));
    Moya::Syntax assignmentOperator = ~(whitespaces & Moya::Syntax::sequence("="));
    Moya::Syntax additiveExpression = multiplicativeExpression & !(whitespaces & ~Moya::Syntax::oneOf("+-") & (additiveExpression | Moya::Syntax::error("Factor expected")));
    Moya::Syntax multiplicativeExpression = exponentalExpression & !(whitespaces & ~Moya::Syntax::oneOf("*/") & (multiplicativeExpression | Moya::Syntax::error("Factor expected")));
    Moya::Syntax exponentalExpression = unaryOperation & !(whitespaces & Moya::Syntax::sequence("^") & (exponentalExpression | Moya::Syntax::error("Factor expected")));
    Moya::Syntax unaryOperation = factor | (whitespaces & ~Moya::Syntax::oneOf("-+") & unaryOperation) | Moya::Syntax::error("Factor expected");
    Moya::Syntax factor = whitespaces & (bracedExpression | functionCall | variable | realNumber);
    Moya::Syntax bracedExpression = Moya::Syntax::sequence("(") & additiveExpression & whitespaces & (Moya::Syntax::sequence(")") | Moya::Syntax::error("Closing bracket expected"));
    Moya::Syntax functionCall = ~identifier & whitespaces & Moya::Syntax::sequence("(") & argumentList & whitespaces & (Moya::Syntax::sequence(")") | Moya::Syntax::error("Closing bracket expected"));
    Moya::Syntax argumentList = ~!(additiveExpression & *(whitespaces & Moya::Syntax::sequence(",") & additiveExpression));
    Moya::Syntax constant = ~(Moya::Syntax::sequence("ans") | Moya::Syntax::sequence("pi") | Moya::Syntax::sequence("e"));
    Moya::Syntax variable = identifier;
    Moya::Syntax identifier = ~(letter & *(letter | digit));
    Moya::Syntax realNumber = +digit & !(Moya::Syntax::sequence(".") & +digit) & !(Moya::Syntax::oneOf("Ee") & !Moya::Syntax::oneOf("+-") & +digit);
    Moya::Syntax whitespaces = ~*Moya::Syntax::oneOf(" \n\r\t\f\v");
    Moya::Syntax letter = ~(Moya::Syntax::range('a', 'z') | Moya::Syntax::range('A', 'Z'));
    Moya::Syntax digit = ~Moya::Syntax::range('0', '9');
};

#endif
