#ifndef ParserH
#define ParserH

#include <string>

#include "Syntax.h"

class Parser
{
    public:

        Parser(const Syntax &syntax)
        {
        }

        bool parse(std::string string)
        {
            return false;
        }

        std::string getErrorString() const
        {
            return "Not implemented";
        }
};

#endif
