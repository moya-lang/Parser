#ifndef ParserH
#define ParserH

#include <string>

#include "Syntax.h"
#include "StringSequencer.h"
#include "ObjectTree.h"

namespace Moya {

class Parser
{
    const Syntax &syntax;
    StringSequencer &stringSequencer;
    ObjectTree &objectTree;

    std::string errorString;

    bool parse(const SyntaxData &syntaxData, std::size_t parent)
    {
        bool visible = syntaxData.visible;
        if (syntaxData.type == SyntaxType::visibility)
            visible = !visible;

        std::size_t objectId = visible ? objectTree.create() : parent;
        std::size_t oldPosition = stringSequencer.position;

        bool success = true;
        if ((syntaxData.type == SyntaxType::reference) || (syntaxData.type == SyntaxType::visibility))
            success = parse(*syntaxData.left, objectId);

        else if (syntaxData.type == SyntaxType::zeroOrOne) {
            parse(*syntaxData.left, objectId);

            if (!errorString.empty())
                success = false;
        }

        else if (syntaxData.type == SyntaxType::oneOrMore) {
            success = false;
            while (parse(*syntaxData.left, objectId))
                success = true;

            if (!errorString.empty())
                success = false;
        }

        else if (syntaxData.type == SyntaxType::zeroOrMore) {
            while (parse(*syntaxData.left, objectId));

            if (!errorString.empty())
                success = false;
        }

        else if (syntaxData.type == SyntaxType::conjunction)
            success = parse(*syntaxData.left, objectId) && parse(*syntaxData.right, objectId);

        else if (syntaxData.type == SyntaxType::alternative)
            success = parse(*syntaxData.left, objectId) || parse(*syntaxData.right, objectId);

        else if (syntaxData.type == SyntaxType::endOfFile)
            success = stringSequencer.isEndOfFile();

        else if (syntaxData.type == SyntaxType::sequence)
            success = stringSequencer.isSequence(syntaxData.content.c_str());

        else if (syntaxData.type == SyntaxType::noneOf)
            success = stringSequencer.isNoneOf(syntaxData.content.c_str());

        else if (syntaxData.type == SyntaxType::oneOf)
            success = stringSequencer.isOneOf(syntaxData.content.c_str());

        else if (syntaxData.type == SyntaxType::range)
            success = stringSequencer.isInRange(syntaxData.content[0], syntaxData.content[1]);

        else {
            if (errorString.empty())
                errorString = syntaxData.content;
            success = false;
        }

        if (visible)
            if (success) {
                Object &object = objectTree.commit(objectId);
                object.syntaxId = syntaxData.id;
                object.parent = parent;
                object.from = oldPosition;
                object.to = stringSequencer.position;
            }
            else
                objectTree.rollback(objectId);

        if (!success)
            stringSequencer.position = oldPosition;

        return success;
    }


    public:

        Parser(const Syntax &syntax, StringSequencer &stringSequencer, ObjectTree &objectTree) :
            syntax(syntax), stringSequencer(stringSequencer), objectTree(objectTree)
        {
        }

        bool parse()
        {
            errorString.clear();
            return parse(syntax.getData(), ObjectTree::nullObject);
        }

        std::string getErrorString() const
        {
            return errorString;
        }
};

}

#endif
