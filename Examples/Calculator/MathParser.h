#ifndef MathParser_H
#define MathParser_H

#include "Parser.h"

class MathParser : public Parser
{
    PARSER_INIT(MathParser, MathExpression)
    {
    }

    PARSER_DEFINE(MathExpression)
    {
        PARSER_PARSE(Assignment);

        if (!PARSER_PARSE(AdditiveExpression))
            return false;

        PARSER_PARSE_SILENT(Whitespaces);

        return PARSER_PARSE_STOP();
    }

    PARSER_DEFINE(Assignment)
    {
        PARSER_PARSE_SILENT(Whitespaces);

        if (!PARSER_PARSE(Variable))
            return false;

        PARSER_PARSE_SILENT(Whitespaces);

        return PARSER_PARSE_SILENT(AssignOperator);
    }

    PARSER_DEFINE(AdditiveExpression)
    {
        if (!PARSER_PARSE(MultiplicativeExpression))
            return false;

        PARSER_PARSE_SILENT(Whitespaces);

        if (!PARSER_PARSE(AdditionOperator) && !PARSER_PARSE(SubtractionOperator))
            return true;

        return PARSER_PARSE(AdditiveExpression);
    }

    PARSER_DEFINE(MultiplicativeExpression)
    {
        if (!PARSER_PARSE(UnaryOperation))
            return false;

        PARSER_PARSE_SILENT(Whitespaces);

        if (!PARSER_PARSE(MultiplyOperator) && !PARSER_PARSE(DivideOperator))
            return true;

        return PARSER_PARSE(MultiplicativeExpression);
    }

    PARSER_DEFINE(UnaryOperation)
    {
        if (PARSER_PARSE(Factor))
            return true;

        PARSER_PARSE_SILENT(Whitespaces);

        if (PARSER_PARSE(SubtractionOperator) || PARSER_PARSE(AdditionOperator))
            return PARSER_PARSE(UnaryOperation);

        return false;
    }

    PARSER_DEFINE(Factor)
    {
        PARSER_PARSE_SILENT(Whitespaces);

        if (PARSER_PARSE(BracedExpression))
            return true;

        if (PARSER_PARSE(FunctionCall))
            return true;

        if (PARSER_PARSE(Variable))
            return true;

        return PARSER_PARSE(RealNumber);
    }

    PARSER_DEFINE(BracedExpression)
    {
        if (!PARSER_PARSE_SILENT(LeftBracket))
            return false;

        if (!PARSER_PARSE(AdditiveExpression))
            return false;

        PARSER_PARSE_SILENT(Whitespaces);

        return PARSER_PARSE_SILENT(RightBracket);
    }

    PARSER_DEFINE(FunctionCall)
    {
        if (!PARSER_PARSE(Identifier))
            return false;

        PARSER_PARSE_SILENT(Whitespaces);

        if (!PARSER_PARSE_SILENT(LeftBracket))
            return false;

        if (PARSER_PARSE_SILENT(RightBracket))
            return true;

        while (true) {

            if (PARSER_PARSE_STOP())
                return false;

            if (!PARSER_PARSE(AdditiveExpression))
                return false;

            PARSER_PARSE_SILENT(Whitespaces);

            if (PARSER_PARSE_SILENT(RightBracket))
                return true;

            if (!PARSER_PARSE_SILENT(CommaSeparator))
                return false;
        }
    }

    PARSER_DEFINE(Variable)
    {
        return PARSER_PARSE_SILENT(Identifier);
    }

    PARSER_DEFINE(Identifier)
    {
        if (!PARSER_PARSE_SILENT(Letter))
            return false;

        while (PARSER_PARSE_SILENT(LetterDigit));
        return true;
    }

    PARSER_DEFINE(LetterDigit)
    {
        if (PARSER_PARSE_SILENT(Letter))
            return true;

        return PARSER_PARSE_SILENT(Digit);
    }

    PARSER_DEFINE(Letter)
    {
        if (PARSER_PARSE_SILENT(UpperCaseLetter))
            return true;

        return PARSER_PARSE_SILENT(LowerCaseLetter);
    }

    PARSER_DEFINE(RealNumber)
    {
        if (!PARSER_PARSE_SILENT(IntegerNumber))
            return false;

        if (PARSER_PARSE_SILENT(DotNumberSeparator))
            if (!PARSER_PARSE_SILENT(IntegerNumber))
                return false;

        if (PARSER_PARSE_SILENT(LowerEngineringNotation) || PARSER_PARSE_SILENT(UpperEngineringNotation)) {

            if (PARSER_PARSE_SILENT(AdditionOperator))
                if (PARSER_PARSE_SILENT(SubtractionOperator))
                    return false;

            if (PARSER_PARSE_SILENT(SubtractionOperator))
                if (PARSER_PARSE_SILENT(AdditionOperator))
                    return false;

            if (!PARSER_PARSE_SILENT(IntegerNumber))
                return false;
        }

        return true;
    }

    PARSER_DEFINE(IntegerNumber)
    {
        if (!PARSER_PARSE_SILENT(Digit))
            return false;

        while (PARSER_PARSE_SILENT(Digit));
        return true;
    }

    PARSER_DEFINE(Whitespaces)
    {
        if (!PARSER_PARSE_SILENT(WhitespaceCharacter))
            return false;

        while (PARSER_PARSE_SILENT(WhitespaceCharacter));
        return true;
    }

    PARSER_DEFINE_SEQUENCE(LeftBracket, "(");
    PARSER_DEFINE_SEQUENCE(RightBracket, ")");
    PARSER_DEFINE_SEQUENCE(CommaSeparator, ",");
    PARSER_DEFINE_SEQUENCE(AssignOperator, "=");
    PARSER_DEFINE_SEQUENCE(MultiplyOperator, "*");
    PARSER_DEFINE_SEQUENCE(DivideOperator, "/");
    PARSER_DEFINE_SEQUENCE(AdditionOperator, "+");
    PARSER_DEFINE_SEQUENCE(SubtractionOperator, "-");
    PARSER_DEFINE_SEQUENCE(DotNumberSeparator, ".");
    PARSER_DEFINE_SEQUENCE(LowerEngineringNotation, "e");
    PARSER_DEFINE_SEQUENCE(UpperEngineringNotation, "E");
    PARSER_DEFINE_ONE_OF(WhitespaceCharacter, " \n\r\t\v\f");
    PARSER_DEFINE_RANGE(UpperCaseLetter, 'A', 'Z');
    PARSER_DEFINE_RANGE(LowerCaseLetter, 'a', 'z');
    PARSER_DEFINE_RANGE(Digit, '0', '9');
};

#endif
