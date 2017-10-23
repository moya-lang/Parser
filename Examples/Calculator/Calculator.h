#ifndef CalculatorH
#define CalculatorH

#include <string>
#include <iostream>

#include "MathParser.h"

class Calculator
{
    MathParser mathParser;

    public:

        Calculator(int argumentCount, char **arguments)
        {
            (void) argumentCount;
            (void) arguments;
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

                if (mathParser.solve(command))
                    std::cout << mathParser.getResult() << std::endl;

                else
                    std::cout << "Error: " << mathParser.getErrorString() << std::endl;
            }
        }
};

#endif
