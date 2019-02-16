#ifndef ObjectTreeH
#define ObjectTreeH

#include <vector>

#include "Object.h"

namespace Moya {

class ObjectTree
{
    std::vector<Object> objects;
    std::size_t objectIndex;

    public:

        static const std::size_t nullObject = std::size_t(-1);

        ObjectTree(size_t initialCapacity = 32 * 1024)
        {
            objects.reserve(initialCapacity);
            reset();
        }

        size_t getNextId()
        {
            return objects.size();
        }

        void create()
        {
            objects.push_back(Object());
        }

        void rollback(size_t id)
        {
            objects.resize(id);
        }

        Object &commit(size_t id)
        {
            return objects[id];
        }

        void reset()
        {
            objectIndex = 0;
            objects.resize(0);
        }

        std::size_t next()
        {
            if (!hasNext())
                return nullObject;

            return objectIndex++;
        }

        bool hasNext() const
        {
            return objectIndex < objects.size();
        }

        const Object &operator *() const
        {
            return objects[objectIndex];
        }

        const Object *operator ->() const
        {
            return &objects[objectIndex];
        }

        const Object &operator [](std::size_t index) const
        {
            return objects[index];
        }
};

}

#endif
