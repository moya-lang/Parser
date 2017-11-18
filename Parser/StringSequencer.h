#ifndef StringSequencerH
#define StringSequencerH

#include <string>

namespace Moya {

class StringSequencer
{
    const std::string &string;


    public:

        std::size_t position = 0;

        StringSequencer(const std::string &string) :
            string(string)
        {
        }

        bool advance()
        {
            if (isEndOfFile())
                return false;

            position++;
            return true;
        }

        bool isEndOfFile()
        {
            return position >= string.size();
        }

        bool isSequence(const char *sequence)
        {
            std::size_t newPosition = position;

            const char *data = string.data();
            while (*sequence && (newPosition < string.size()))
                if (*(sequence++) != data[newPosition++])
                    return false;

            if (*sequence)
                return false;

            position = newPosition;
            return true;
        }

        bool isNoneOf(const char *set)
        {
            if (isEndOfFile())
                return false;

            const char *data = string.data();
            char character = data[position];
            while (*set)
                if (character == *(set++))
                    return false;

            position++;
            return true;
        }

        bool isOneOf(const char *set)
        {
            if (isEndOfFile())
                return false;

            const char *data = string.data();
            char character = data[position];
            while (*set)
                if (character == *(set++)) {
                    position++;
                    return true;
                }

            return false;
        }

        bool isInRange(char from, char to)
        {
            if (isEndOfFile())
                return false;

            const char *data = string.data();
            unsigned char character = static_cast<unsigned char>(data[position]);
            if ((character >= static_cast<unsigned char>(from)) && (character <= static_cast<unsigned char>(to))) {
                position++;
                return true;
            }

            return false;
        }
};

}

#endif
