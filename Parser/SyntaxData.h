#ifndef SyntaxDataH
#define SyntaxDataH

#include <memory>
#include <string>
#include <atomic>

#include "SyntaxType.h"
#include "CopyPointer.h"

namespace Moya {

class SyntaxData
{
    public:

        unsigned id;
        bool visible;
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
            id(generateUniqueSyntaxId()), visible(true), type(syntaxType), content(content)
        {
        }

        SyntaxData(const SyntaxData &data) = default;
        SyntaxData(SyntaxData &&data) noexcept = default;
        SyntaxData &operator =(const SyntaxData &data) = default;
        SyntaxData &operator =(SyntaxData &&data) = default;
};

}

#endif
