#ifndef SyntaxH
#define SyntaxH

#include <string>
#include <utility>

#include "SyntaxData.h"

class Syntax
{
    SyntaxData data;

    Syntax &operator =(const Syntax &syntax) = delete;
    Syntax &operator =(Syntax &&syntax) = delete;

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

        data.left->visible = !data.left->visible;
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

        data.left->visible = !data.left->visible;
    }

    Syntax(const Syntax &left, Syntax &&right, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = left.data;
        data.right = std::move(right.data);

        data.right->visible = !data.right->visible;
    }

    Syntax(Syntax &&left, Syntax &&right, SyntaxType syntaxType) noexcept :
        data(syntaxType)
    {
        data.left = std::move(left.data);
        data.right = std::move(right.data);

        data.left->visible = !data.left->visible;
        data.right->visible = !data.right->visible;
    }


    public:

        Syntax(const Syntax &syntax) noexcept :
            data(SyntaxType::reference)
        {
            data.left = syntax.data;
        }

        Syntax(Syntax &&syntax) noexcept = default;

        bool operator ==(std::size_t syntaxId) const noexcept
        {
            return syntaxId == data.id;
        }

        Syntax operator ~() const & noexcept
        {
            return Syntax(*this, SyntaxType::visibility);
        }

        Syntax operator ~() && noexcept
        {
            return Syntax(std::move(*this), SyntaxType::visibility);
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

        static Syntax endOfFile() noexcept
        {
            return Syntax(SyntaxType::endOfFile);
        }

        static Syntax sequence(std::string content) noexcept
        {
            return Syntax(SyntaxType::sequence, content);
        }

        static Syntax oneOf(std::string content) noexcept
        {
            return Syntax(SyntaxType::oneOf, content);
        }

        static Syntax range(char from, char to) noexcept
        {
            return Syntax(SyntaxType::range, { from, to });
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
