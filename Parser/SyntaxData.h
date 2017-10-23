#ifndef SyntaxDataH
#define SyntaxDataH

#include <memory>
#include <string>
#include <atomic>

#include "SyntaxType.h"
#include "CopyPointer.h"

struct SyntaxData
{
    unsigned id;
    SyntaxType type;
    std::string content;

    CopyPointer<SyntaxData> left;
    CopyPointer<SyntaxData> right;

    unsigned generateUniqueSyntaxId()
    {
        static std::atomic<unsigned> uniqueSyntaxId = 0;
        return ++uniqueSyntaxId;
    }

    SyntaxData(SyntaxType syntaxType, std::string content = std::string()) :
        id(generateUniqueSyntaxId()), type(syntaxType), content(content)
    {
    }

    SyntaxData(const SyntaxData &data) :
        id(data.id), type(data.type), content(data.content), left(data.left), right(data.right)
    {
    }

    SyntaxData(SyntaxData &&data) noexcept :
        id(std::move(data.id)), type(std::move(data.type)), content(std::move(data.content)), left(std::move(data.left)), right(std::move(data.right))
    {
    }

    SyntaxData &operator =(const SyntaxData &data)
    {
        id = data.id;
        type = data.type;
        content = data.content;
        left = data.left;
        right = data.right;
    }

    SyntaxData &operator =(SyntaxData &&data)
    {
        id = std::move(data.id);
        type = std::move(data.type);
        content = std::move(data.content);
        left = std::move(data.left);
        right = std::move(data.right);
    }
};

#endif
