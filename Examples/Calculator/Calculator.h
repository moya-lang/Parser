#ifndef Calculator_H
#define Calculator_H

#include <map>
#include <string>
#include <iostream>

#include "CalculatorSolver.h"

class Calculator
{
	MathFunctions mathFunctions;
	std::map<std::string, double> variables;

	bool solve(const std::string &expression)
	{
		ObjectTree objectTree;
		StringSequencer sequencer(expression);
		MathParser mathParser(objectTree, sequencer);
		CalculatorSolver solver(objectTree, mathParser, expression, mathFunctions, variables);

		if (!mathParser.parse()) {
			std::cout << "Error: Invalid syntax" << std::endl;
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

		Calculator(char **arguments, int numberOfArguments)
		{
            variables["e"] = 2.7182818284590452;
            variables["pi"] = 3.1415926535897932;
		}

		int run()
		{
			std::cout << "Calculator" << std::endl;
			std::cout << "Varsion: 1.0" << std::endl;

			while (true) {

				std::string command;
				std::cout << std::endl << ": ";
				std::getline(std::cin, command);

				if (command == "exit")
					return 0;

				if (command == "")
					continue;

				solve(command);
			}
		}
};

#endif
