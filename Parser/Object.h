#ifndef ObjectH
#define ObjectH

#include <cstddef>

class Object
{
    public:

        std::size_t syntaxId = 0;
        std::size_t parent = 0;
        std::size_t from = 0;
        std::size_t to = 0;
};

#endif
