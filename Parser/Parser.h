#ifndef Parser_H
#define Parser_H

#include <cstddef>
#include <functional>

#include "ObjectTree.h"
#include "ParserMacros.h"
#include "StringSequencer.h"

class Parser
{
	protected:

		size_t typeId = 0;
		ObjectTree &objectTree;
		StringSequencer &stringSequencer;

		bool parseElement(std::function<bool(size_t)> onParse, size_t type, size_t parent, bool silent)
		{
			size_t oldPosition = stringSequencer.position;
			size_t objectId = objectTree.create();

			bool status = onParse(objectId);

			if (!status || silent)
				objectTree.rollback(objectId);

			else {
				Object &object = objectTree.commit(objectId);
				object.type = type;
				object.parent = parent;
				object.from = oldPosition;
				object.to = stringSequencer.position;
			}

			if (!status)
				stringSequencer.position = oldPosition;

			return status;
		}


	public:

		static const size_t nullParent = size_t(-1);

		Parser(ObjectTree &objectTree, StringSequencer &stringSequencer) :
			objectTree(objectTree), stringSequencer(stringSequencer)
		{
		}

		size_t getNumberOfTypes() const
		{
			return typeId;
		}

		virtual bool parse() = 0;
};

#endif
