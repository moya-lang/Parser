#ifndef Object_H
#define Object_H

#include <cstddef>

class Object
{
	public:

		size_t type = 0;
		size_t parent = 0;
		size_t from = 0;
		size_t to = 0;
};

#endif