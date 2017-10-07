#ifndef SyntaxTypeH
#define SyntaxTypeH

enum class SyntaxType : int
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
    hidden,
    error,
};

#endif
