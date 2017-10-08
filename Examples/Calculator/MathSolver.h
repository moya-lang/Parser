#ifndef MathSolverH
#define MathSolverH

#include <map>
#include <string>

#include "MathParser.h"
#include "MathFunctions.h"

class MathSolver
{
    MathParser &parser;
    MathFunctions mathFunctions;
    std::map<std::string, double> variables;


    public:

        MathSolver(MathParser &parser) :
            parser(parser)
        {
            variables["e"] = 2.7182818284590452;
            variables["pi"] = 3.1415926535897932;
        }

        bool solve()
        {
            return false;
        }

        std::string getErrorString() const
        {
            return "Not implemented";
        }

        double getResult()
        {
            return 0;
        }
};

#endif
