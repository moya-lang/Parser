#ifndef SyntaxTypeH
#define SyntaxTypeH

namespace Moya {

enum class SyntaxType : std::size_t
{
    reference,
    visibility,
    zeroOrOne,
    oneOrMore,
    zeroOrMore,
    conjunction,
    alternative,
    endOfFile,
    advance,
    sequence,
    other,
    oneOf,
    noneOf,
    range,
    positive,
    negative,
    error,
};

}

#endif
