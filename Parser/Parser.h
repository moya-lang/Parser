#ifndef ParserH
#define ParserH

#include <string>

#include "Syntax.h"
#include "StringSequencer.h"
#include "ObjectTree.h"

class Parser
{
    const Syntax &syntax;
	StringSequencer &stringSequencer;
	ObjectTree &objectTree;

	std::string errorString;
	static const size_t nullParent = size_t(-1);

    bool validate(const SyntaxData &syntaxData)
    {/*
        if (syntaxData.type == SyntaxType::range)
            return ((syntaxData.left->type == SyntaxType::character) && (syntaxData.right->type == SyntaxType::character));

        if (syntaxData.left)
            if (!validate(*syntaxData.left))
                return false;

        if (syntaxData.right)
            if (!validate(*syntaxData.right))
                return false;
        */
        return true;
    }

    bool parse(const SyntaxData &syntaxData, std::size_t parent)
    {
        std::size_t oldPosition = stringSequencer.position;
        std::size_t objectId = objectTree.create();

        bool status = true;
        if ((syntaxData.type == SyntaxType::reference) || (syntaxData.type == SyntaxType::silent))
            status = parse(*syntaxData.left, objectId);

        else if ((syntaxData.type == SyntaxType::character) || (syntaxData.type == SyntaxType::sequence))
            status = stringSequencer.isSequence(syntaxData.content.c_str());

        else if (syntaxData.type == SyntaxType::zeroOrOne)
            parse(*syntaxData.left, objectId);

        else if (syntaxData.type == SyntaxType::oneOrMore) {
            status = false;
            while (parse(*syntaxData.left, objectId))
                status = true;
        }

        else if (syntaxData.type == SyntaxType::zeroOrMore) {
            while (parse(*syntaxData.left, objectId));
        }

        else if (syntaxData.type == SyntaxType::conjunction)
            status = parse(*syntaxData.left, objectId) && parse(*syntaxData.right, objectId);

        else if (syntaxData.type == SyntaxType::alternative)
            status = parse(*syntaxData.left, objectId) || parse(*syntaxData.right, objectId);

        else if (syntaxData.type == SyntaxType::range)
            status = stringSequencer.isInRange(syntaxData.left->content[0], syntaxData.right->content[0]);

        else {
            errorString = syntaxData.content;
            status = false;
        }

        if (!status/* || (syntaxData.type == SyntaxType::silent)*/)
            objectTree.rollback(objectId);

        else {
            Object &object = objectTree.commit(objectId);
            object.syntaxId = syntaxData.id;
            object.parent = parent;
            object.from = oldPosition;
            object.to = stringSequencer.position;
        }

        if (!status)
            stringSequencer.position = oldPosition;

        return status;
    }


    public:

        Parser(const Syntax &syntax, StringSequencer &stringSequencer, ObjectTree &objectTree) :
            syntax(syntax), stringSequencer(stringSequencer), objectTree(objectTree)
        {
        }

        bool parse()
        {
            if (!validate(syntax.getData())) {
                errorString = "Range on non-characters";
                return false;
            }

			return parse(syntax.getData(), Parser::nullParent);
        }

        std::string getErrorString() const
        {
            return errorString;
        }
};

#endif
