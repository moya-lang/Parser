#ifndef StringSequencerH
#define StringSequencerH

#include <string>

class StringSequencer
{
    const std::string &string;

    public:

        size_t position = 0;

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

        bool isInRange(char from, char to)
        {
            if (isEndOfFile())
                return false;

            const char *data = string.data();
            char character = data[position];
            if ((character >= from) && (character <= to)) {
                position++;
                return true;
            }

            return false;
        }

        bool isOneOf(const char *set)
        {
            if (position >= string.size())
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

        bool isSequence(const char *sequence)
        {
            size_t newPosition = position;

            const char *data = string.data();
            while (*sequence && (newPosition < string.size()))
                if (*(sequence++) != data[newPosition++])
                    return false;

            if (*sequence)
                return false;

            position = newPosition;
            return true;
        }
};

#endif
