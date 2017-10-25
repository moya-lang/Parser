#ifndef SyntaxTypeH
#define SyntaxTypeH

enum class SyntaxType : std::size_t
{
    reference,
    character,
    sequence,
    silent,
    zeroOrOne,
    oneOrMore,
    zeroOrMore,
    conjunction,
    alternative,
    range,
    error,
};

#endif
