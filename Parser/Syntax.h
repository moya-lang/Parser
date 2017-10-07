#ifndef SyntaxH
#define SyntaxH

#include <utility>

#include "SyntaxData.h"

class Syntax
{
    SyntaxData data;

    Syntax &operator =(const Syntax &syntax) noexcept;
    Syntax &operator =(Syntax &&syntax) noexcept;

    Syntax(SyntaxType syntaxType, std::string content = std::string()) :
        data(syntaxType, content)
    {
    }

    Syntax(const Syntax &left, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = left.data;
    }

    Syntax(Syntax &&left, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = std::move(left.data);
    }

    Syntax(const Syntax &left, const Syntax &right, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = left.data;
        data.right = right.data;
    }

    Syntax(Syntax &&left, const Syntax &right, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = std::move(left.data);
        data.right = right.data;
    }

    Syntax(const Syntax &left, Syntax &&right, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = left.data;
        data.right = std::move(right.data);
    }

    Syntax(Syntax &&left, Syntax &&right, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = std::move(left.data);
        data.right = std::move(right.data);
    }


    public:

        ~Syntax() noexcept
        {
        }

        explicit Syntax(int character) noexcept :
            data(SyntaxType::character, std::string(1, (char) character))
        {
        }

        explicit Syntax(char character) noexcept :
            data(SyntaxType::character, std::string(1, character))
        {
        }

        explicit Syntax(std::string sequence) noexcept :
            data(SyntaxType::sequence, sequence)
        {
        }

        Syntax(const Syntax &syntax) noexcept :
            data(SyntaxType::reference)
        {
            data.left = syntax.data;
        }

        Syntax(Syntax &&syntax) noexcept :
            data(std::move(syntax.data))
        {
        }

        Syntax operator ~() const & noexcept
        {
            return Syntax(*this, SyntaxType::silent);
        }

        Syntax operator ~() && noexcept
        {
            return Syntax(std::move(*this), SyntaxType::silent);
        }

        Syntax operator !() const & noexcept
        {
            return Syntax(*this, SyntaxType::zeroOrOne);
        }

        Syntax operator !() && noexcept
        {
            return Syntax(std::move(*this), SyntaxType::zeroOrOne);
        }

        Syntax operator +() const & noexcept
        {
            return Syntax(*this, SyntaxType::oneOrMore);
        }

        Syntax operator +() && noexcept
        {
            return Syntax(std::move(*this), SyntaxType::oneOrMore);
        }

        Syntax operator *() const & noexcept
        {
            return Syntax(*this, SyntaxType::zeroOrMore);
        }

        Syntax operator *() && noexcept
        {
            return Syntax(std::move(*this), SyntaxType::zeroOrMore);
        }

        Syntax operator &(const Syntax &syntax) const & noexcept
        {
            return Syntax(*this, syntax, SyntaxType::conjunction);
        }

        Syntax operator &(const Syntax &syntax) && noexcept
        {
            return Syntax(std::move(*this), syntax, SyntaxType::conjunction);
        }

        Syntax operator &(Syntax &&syntax) const & noexcept
        {
            return Syntax(*this, std::move(syntax), SyntaxType::conjunction);
        }

        Syntax operator &(Syntax &&syntax) && noexcept
        {
            return Syntax(std::move(*this), std::move(syntax), SyntaxType::conjunction);
        }

        Syntax operator |(const Syntax &syntax) const & noexcept
        {
            return Syntax(*this, syntax, SyntaxType::alternative);
        }

        Syntax operator |(const Syntax &syntax) && noexcept
        {
            return Syntax(std::move(*this), syntax, SyntaxType::alternative);
        }

        Syntax operator |(Syntax &&syntax) const & noexcept
        {
            return Syntax(*this, std::move(syntax), SyntaxType::alternative);
        }

        Syntax operator |(Syntax &&syntax) && noexcept
        {
            return Syntax(std::move(*this), std::move(syntax), SyntaxType::alternative);
        }

        Syntax operator -(const Syntax &syntax) const & noexcept
        {
            return Syntax(*this, syntax, SyntaxType::range);
        }

        Syntax operator -(const Syntax &syntax) && noexcept
        {
            return Syntax(std::move(*this), syntax, SyntaxType::range);
        }

        Syntax operator -(Syntax &&syntax) const & noexcept
        {
            return Syntax(*this, std::move(syntax), SyntaxType::range);
        }

        Syntax operator -(Syntax &&syntax) && noexcept
        {
            return Syntax(std::move(*this), std::move(syntax), SyntaxType::range);
        }

        static Syntax hidden(std::string content) noexcept
        {
            return Syntax(SyntaxType::hidden, content);
        }

        static Syntax error(std::string content) noexcept
        {
            return Syntax(SyntaxType::error, content);
        }

        const SyntaxData &getData() const
        {
            return data;
        }
};

#endif
