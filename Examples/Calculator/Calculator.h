#ifndef CalculatorH
#define CalculatorH

#include <string>
#include <iostream>

#include "MathParser.h"
#include "MathSolver.h"

class Calculator
{
    MathParser parser;
    MathSolver solver;

    bool solve(const std::string &expression)
    {
        if (!parser.parse(expression)) {
            std::cout << "Error: " << parser.getErrorString() << std::endl;
            return false;
        }

        if (!solver.solve()) {
            std::cout << "Error: " << solver.getErrorString() << std::endl;
            return false;
        }

        std::cout << solver.getResult() << std::endl;
        return true;
    }


    public:

        Calculator(int argumentCount, char **arguments) :
            solver(parser)
        {
        }

        int run()
        {
            std::cout << "Calculator" << std::endl;
            std::cout << "Version: 1.1" << std::endl;

            while (true) {

                std::string command;
                std::cout << std::endl << ": ";
                std::getline(std::cin, command);

                if (command == "")
                    continue;

                if (command == "exit")
                    return 0;

                solve(command);
            }
        }
};

#endif
