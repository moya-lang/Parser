#ifndef MathFunctions_H
#define MathFunctions_H

#include <cmath>
#include <vector>
#include <string>

class MathFunctions
{
    double result;
    std::string errorString;

    template <typename... Types>
    double execute(double(*func)(Types...), const std::vector<double> &params)
    {
        return execute(func, params, std::index_sequence_for<Types...>());
    }

    template <typename... Types, std::size_t... Is>
    bool execute(double (*func)(Types...), const std::vector<double> &params, std::index_sequence<Is...>)
    {
        if (params.size() != sizeof...(Types)) {
            errorString = "Wrong number of function arguments";
            return false;
        }

        errno = 0;
        result = func(params[Is]...);
        errorString = strerror(errno);
        return !errno;
    }


    public:

        bool execute(const std::string &functionName, const std::vector<double> &params)
        {
            if (functionName == "cos") return execute<double>(cos, params);
            if (functionName == "sin") return execute<double>(sin, params);
            if (functionName == "tan") return execute<double>(tan, params);
            if (functionName == "acos") return execute<double>(acos, params);
            if (functionName == "asin") return execute<double>(asin, params);
            if (functionName == "atan") return execute<double>(atan, params);
            if (functionName == "atan2") return execute<double, double>(atan2, params);

            if (functionName == "cosh") return execute<double>(cosh, params);
            if (functionName == "sinh") return execute<double>(sinh, params);
            if (functionName == "tanh") return execute<double>(tanh, params);
            if (functionName == "acosh") return execute<double>(acosh, params);
            if (functionName == "asinh") return execute<double>(asinh, params);
            if (functionName == "atanh") return execute<double>(atanh, params);

            if (functionName == "exp") return execute<double>(exp, params);
            if (functionName == "exp2") return execute<double>(exp2, params);
            if (functionName == "log") return execute<double>(log, params);
            if (functionName == "log2") return execute<double>(log2, params);
            if (functionName == "pow") return execute<double, double>(pow, params);
            if (functionName == "sqrt") return execute<double>(sqrt, params);
            if (functionName == "cbrt") return execute<double>(cbrt, params);
            if (functionName == "hypot") return execute<double, double>(hypot, params);

            if (functionName == "erf") return execute<double>(erf, params);
            if (functionName == "erfc") return execute<double>(erfc, params);
            if (functionName == "tgamma") return execute<double>(tgamma, params);
            if (functionName == "lgamma") return execute<double>(lgamma, params);

            if (functionName == "ceil") return execute<double>(ceil, params);
            if (functionName == "floor") return execute<double>(floor, params);
            if (functionName == "mod") return execute<double, double>(fmod, params);
            if (functionName == "trunc") return execute<double>(trunc, params);
            if (functionName == "round") return execute<double>(round, params);

            if (functionName == "dim") return execute<double, double>(fdim, params);
            if (functionName == "max") return execute<double, double>(fmax, params);
            if (functionName == "min") return execute<double, double>(fmin, params);
            if (functionName == "abs") return execute<double>(abs, params);

            errorString = "Unknown function name";
            return false;
        }

        double getResult() const
        {
            return result;
        }

        const char *getErrorString() const
        {
            return errorString.c_str();
        }
};

#endif
