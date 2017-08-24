#ifndef ObjectTree_H
#define ObjectTree_H

#include <vector>

#include "Object.h"

class ObjectTree : public std::vector<Object>
{
	public:

		ObjectTree(size_t initialCapacity = 32 * 1024)
		{
			reserve(initialCapacity);
		}

		size_t create()
		{
			push_back(Object());
			return size() - 1;
		}

		void rollback(size_t id)
		{
			resize(id);
		}

		Object &commit(size_t id)
		{
			return (*this)[id];
		}
};

#endif
