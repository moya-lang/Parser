#ifndef CalculatorSolver_H
#define CalculatorSolver_H

#include <map>
#include <string>
#include <vector>

#include "MathParser.h"
#include "MathFunctions.h"

class CalculatorSolver
{
    const ObjectTree &objectTree;
    const MathParser &mathParser;
    const std::string &expression;

    MathFunctions &mathFunctions;
    std::map<std::string, double> &variables;

    size_t objectId = 0;
    double resultContainer = 0;
    double *result = &resultContainer;
    std::string errorString = "No error";

    bool evaluateMathExpression()
    {
        if (objectTree[++objectId].type == mathParser.typeAssignment)
            evaluateAssignment();

        return evaluateAdditiveExpression(*result);
    }

    bool evaluateAssignment()
    {
        const Object &variable = objectTree[++objectId];
        std::string variableName(expression, variable.from, variable.to - variable.from);
        result = &variables[variableName];

        objectId++;
        return true;
    }

    bool evaluateAdditiveExpression(double &result)
    {
        size_t parent = objectId++;
        if (!evaluateMultiplicativeExpression(result))
            return false;

        if (objectId >= objectTree.size())
            return true;

        const Object &operation = objectTree[objectId];
        if (operation.parent != parent)
            return true;

        objectId++;
        double secondArgument;
        if (!evaluateAdditiveExpression(secondArgument))
            return false;

        if (operation.type == mathParser.typeAdditionOperator)
            result += secondArgument;
        else
            result -= secondArgument;

        return true;
    }

    bool evaluateMultiplicativeExpression(double &result)
    {
        size_t parent = objectId++;
        if (!evaluateUnaryOperation(result))
            return false;

        if (objectId >= objectTree.size())
            return true;

        const Object &operation = objectTree[objectId];
        if (operation.parent != parent)
            return true;

        objectId++;
        double secondArgument;
        if (!evaluateMultiplicativeExpression(secondArgument))
            return false;

        if (operation.type == mathParser.typeMultiplyOperator)
            result *= secondArgument;
        else
            result /= secondArgument;

        return true;
    }

    bool evaluateUnaryOperation(double &result)
    {
        const Object &object = objectTree[++objectId];
        if (object.type == mathParser.typeFactor)
            return evaluateFactor(result);

        objectId++;
        if (!evaluateUnaryOperation(result))
            return false;

        if (object.type == mathParser.typeSubtractionOperator)
            result = -result;

        return true;
    }

    bool evaluateFactor(double &result)
    {
        const Object &object = objectTree[++objectId];

        if (object.type == mathParser.typeBracedExpression)
            return evaluateBracedExpression(result);

        if (object.type == mathParser.typeFunctionCall)
            return evaluateFunctionCall(result);

        if (object.type == mathParser.typeVariable)
            return evaluateVariable(result);

        return evaluateRealNumber(result);
    }

    bool evaluateBracedExpression(double &result)
    {
        objectId++;
        return evaluateAdditiveExpression(result);
    }

    bool evaluateFunctionCall(double &result)
    {
        size_t parent = objectId;
        const Object &function = objectTree[++objectId];
        std::string functionName(expression, function.from, function.to - function.from);

        std::vector<double> params;
        params.reserve(5);

        objectId++;
        while (objectId < objectTree.size()) {
            if (objectTree[objectId].parent != parent)
                break;

            params.push_back(0);
            if (!evaluateAdditiveExpression(params.back()))
                return false;
        }

        return executeFunction(result, functionName, params);
    }

    bool executeFunction(double &result, std::string &functionName, std::vector<double> &params)
    {
        if (!mathFunctions.execute(functionName, params)) {
            errorString = mathFunctions.getErrorString();
            return false;
        }

        result = mathFunctions.getResult();
        return true;
    }

    bool evaluateVariable(double &result)
    {
        const Object &variable = objectTree[objectId++];
        std::string variableName(expression, variable.from, variable.to - variable.from);

        if (variables.find(variableName) == variables.end()) {
            errorString = std::string("Unknown variable '") + variableName + "'";
            return false;
        }

        result = variables[variableName];
        return true;
    }

    bool evaluateRealNumber(double &result)
    {
        const Object &realNumber = objectTree[objectId++];
        std::string valueStr(expression, realNumber.from, realNumber.to - realNumber.from);
        result = std::stod(valueStr.c_str());
        return true;
    }


    public:

        CalculatorSolver(const ObjectTree &objectTree, const MathParser &mathParser, const std::string &expression,
            MathFunctions &mathFunctions, std::map<std::string, double> &variables) :
            objectTree(objectTree), mathParser(mathParser), expression(expression),
            mathFunctions(mathFunctions), variables(variables)
        {
        }

        bool solve()
        {
            return evaluateMathExpression();
        }

        double getResult() const
        {
            return *result;
        }

        const char *getErrorString() const
        {
            return errorString.c_str();
        }
};

#endif
