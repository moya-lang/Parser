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
    sequence,
    oneOf,
    range,
    error,
};

}

#endif
