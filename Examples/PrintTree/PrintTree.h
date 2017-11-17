#ifndef PrintTreeH
#define PrintTreeH

#include <iostream>
#include <iomanip>
#include <map>

#include "Parser.h"

class PrintTree
{
    Moya::Syntax expression = (term & ~Moya::Syntax::oneOf("+-") & expression) | term;
    Moya::Syntax term = (factor & ~Moya::Syntax::oneOf("*/") & term) | factor;
    Moya::Syntax factor = constant | variable | (Moya::Syntax::sequence("(") & expression & Moya::Syntax::sequence(")"));
    Moya::Syntax constant = *Moya::Syntax::range('0', '9');
    Moya::Syntax variable = Moya::Syntax::oneOf("xyz");

    void print(std::string &content, Moya::ObjectTree &objectTree)
    {
        std::map<std::size_t, std::string> names;
        names[expression.getData().id] = "expression";
        names[term.getData().id] = "term";
        names[factor.getData().id] = "factor";
        names[variable.getData().id] = "variable";
        names[constant.getData().id] = "constant";

        for (std::size_t objectId = 0; objectTree.hasNext(); objectId = objectTree.next()) {

            int depth = 0;
            for (std::size_t parent = objectTree->parent; parent != objectTree.nullObject; parent = objectTree[parent].parent)
                depth++;

            std::cout << std::setfill('0') << std::setw(4) << objectId;
            std::cout << ": " << std::string(depth, ' ');

            if (names.find(objectTree->syntaxId) != names.end())
                std::cout << names[objectTree->syntaxId].c_str();
            std::cout << "[" << objectTree->syntaxId << "]";

            std::cout << ", '" << std::string(&content.c_str()[objectTree->from], &content.c_str()[objectTree->to]) << "'";
            std::cout << std::endl;
        }
    }


    public:

        PrintTree(int argumentCount, char **arguments)
        {
            (void) argumentCount;
            (void) arguments;
        }

        int run()
        {
            std::string content = "1+2+3";

            Moya::ObjectTree objectTree;
            Moya::StringSequencer stringSequencer(content);
            Moya::Parser parser(expression, stringSequencer, objectTree);

            if (parser.parse())
                print(content, objectTree);
            else
                std::cout << parser.getErrorString() << std::endl;

            return 0;
        }
};

#endif
