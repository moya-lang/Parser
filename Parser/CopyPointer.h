#ifndef CopyPointerH
#define CopyPointerH

#include <utility>

template <typename Type>
class CopyPointer
{
    Type *dynamic;
    Type *pointer;


    public:

        ~CopyPointer()
        {
            delete dynamic;
        }

        CopyPointer() :
            dynamic(nullptr), pointer(nullptr)
        {
        }

        CopyPointer(const CopyPointer<Type> &copyPointer) :
            dynamic(nullptr), pointer(copyPointer.pointer)
        {
        }

        CopyPointer(CopyPointer<Type> &&copyPointer) noexcept :
            dynamic(std::move(copyPointer.dynamic)), pointer(std::move(copyPointer.pointer))
        {
            copyPointer.dynamic = nullptr;
        }

        CopyPointer(const Type &data) :
            dynamic(nullptr), pointer(&data)
        {
        }

        CopyPointer(Type &&data) noexcept :
            dynamic(new Type(std::move(data))), pointer(dynamic)
        {
        }

        CopyPointer &operator =(const CopyPointer<Type> &copyPointer)
        {
            delete dynamic;

            dynamic = nullptr;
            pointer = copyPointer.pointer;

            return *this;
        }

        CopyPointer &operator =(CopyPointer<Type> &&copyPointer) noexcept
        {
            delete dynamic;

            dynamic = std::move(copyPointer.dynamic);
            pointer = std::move(copyPointer.pointer);
            copyPointer.dynamic = nullptr;

            return *this;
        }

        CopyPointer &operator =(const Type &data)
        {
            delete dynamic;

            dynamic = nullptr;
            pointer = const_cast<Type *>(&data); // TODO: Lose qualifier

            return *this;
        }

        CopyPointer &operator =(Type &&data) noexcept
        {
            delete dynamic;

            dynamic = new Type(std::move(data));
            pointer = dynamic;

            return *this;
        }

        Type &operator *() const
        {
            return *pointer;
        }

        Type *operator ->() const
        {
            return pointer;
        }

        operator bool() const
        {
            return (pointer != nullptr);
        }
};

#endif
