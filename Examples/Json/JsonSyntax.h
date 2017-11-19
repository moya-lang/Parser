#ifndef JsonSyntaxH
#define JsonSyntaxH

#include "Parser.h"

struct JsonSyntax
{
    Moya::Syntax json = whitespaces & object & whitespaces & (Moya::Syntax::endOfFile() | Moya::Syntax::error("End of file expected"));
    Moya::Syntax object = Moya::Syntax::sequence("{") & whitespaces & !members & whitespaces & (Moya::Syntax::sequence("}") | Moya::Syntax::error("Closing curly bracket expected"));
    Moya::Syntax members = ~(pair & *(whitespaces & Moya::Syntax::sequence(",") & whitespaces & pair));
    Moya::Syntax pair = ~string & whitespaces & Moya::Syntax::sequence(":") & whitespaces & value;
    Moya::Syntax array = Moya::Syntax::sequence("[") & whitespaces & !elements & whitespaces & (Moya::Syntax::sequence("]") | Moya::Syntax::error("Closing square bracket expected"));
    Moya::Syntax elements = ~(value & *(whitespaces & Moya::Syntax::sequence(",") & whitespaces & value));
    Moya::Syntax whitespaces = ~*Moya::Syntax::oneOf(" \t\n\v\f\r");
    Moya::Syntax value = string | number | object | array |
        Moya::Syntax::sequence("true") | Moya::Syntax::sequence("false") | Moya::Syntax::sequence("null");
    Moya::Syntax string = ~(Moya::Syntax::sequence("\"") & *character &
        (Moya::Syntax::endOfFile() & Moya::Syntax::error("Unexpected end of string") | Moya::Syntax::sequence("\"")));
    Moya::Syntax character = ~(
        Moya::Syntax::sequence("\\/") | Moya::Syntax::sequence("\\b") | Moya::Syntax::sequence("\\f") |
        Moya::Syntax::sequence("\\n") | Moya::Syntax::sequence("\\r") | Moya::Syntax::sequence("\\t") |
        Moya::Syntax::sequence("\\\"") | Moya::Syntax::sequence("\\\\") |
        Moya::Syntax::sequence("\\u") & (hexDigit & hexDigit & hexDigit & hexDigit | Moya::Syntax::error("Expected sequence of four hexadecimal digits")) |
        Moya::Syntax::sequence("\\") & Moya::Syntax::error("Invalid escape sequence") |
        Moya::Syntax::noneOf("\""));
    Moya::Syntax hexDigit = ~(Moya::Syntax::range('0', '9') | Moya::Syntax::range('a', 'f') | Moya::Syntax::range('A', 'F'));
    Moya::Syntax number = ~(
        !Moya::Syntax::sequence("-") & (Moya::Syntax::range('1', '9') & *digit | digit) &
        !(Moya::Syntax::sequence(".") & (+digit | Moya::Syntax::error("Expected number fraction"))) &
        !(Moya::Syntax::oneOf("Ee") & !Moya::Syntax::oneOf("+-") & (+digit | Moya::Syntax::error("Expected number exponent"))));
    Moya::Syntax digit = ~Moya::Syntax::range('0', '9');
};

#endif
