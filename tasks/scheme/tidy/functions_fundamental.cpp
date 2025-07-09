#include "functions_declaration.h"

static std::map<std::string, AbstractFunction> functions_list = {
    {"quote", QuoteFunction()},

    {"=", CompareFunction([](long long a, long long b) { return (a == b); })},
    {"<", CompareFunction([](long long a, long long b) { return (a < b); })},
    {">", CompareFunction([](long long a, long long b) { return (a > b); })},
    {"<=", CompareFunction([](long long a, long long b) { return (a <= b); })},
    {">=", CompareFunction([](long long a, long long b) { return (a >= b); })},

    {"number?", IsNumberFunction()},

    {"+",
     DirectArithmeticFunction([](long long a, long long b) { return a + b; }, 0)},
    {"-", ReverseArithmeticFunction(
              [](long long a, long long b) { return a - b; }, 0)},
    {"*",
     DirectArithmeticFunction([](long long a, long long b) { return a * b; }, 1)},
    {"/", ReverseArithmeticFunction(
              [](long long a, long long b) { return a / b; }, 1)},

    {"max",
     ExtremumFunction([](long long a, long long b) { return std::max(a, b); })},
    {"min",
     ExtremumFunction([](long long a, long long b) { return std::min(a, b); })},

    {"abs", AbsFunction()},

    {"boolean?", IsBooleanFunction()},

    {"not", NotFunction()},
    {"and", AndFunction()},
    {"or", OrFunction()},

    {"pair?", IsPairFunction()},
    {"null?", IsNullFunction()},
    {"list?", IsListFunction()},

    {"cons", ConsFunction()},
    {"car", CarFunction()},
    {"cdr", CdrFunction()},

    {"list", ListFunction()},
    {"list-ref", ListRefFunction()},
    {"list-tail", ListTailFunction()},

    {"if", IfFunction()},
    {"symbol?", IsSymbolFunction()},
    {"define", DefineFunction()},
    {"set!", SetFunction()},
    {"set-car!", SetCarFunction()},
    {"set-cdr!", SetCdrFunction()},
    {"lambda", LambdaFunction()}
};

AbstractFunction* GetFunction(std::shared_ptr<Object> obj) {
    return &functions_list[As<Symbol>(obj)->GetName()];
}

bool IsFunction(std::string name) {
    return functions_list.count(name) != 0;
}

std::shared_ptr<Object> QuoteFunction::operator()(std::shared_ptr<Object> argument) {
    return As<Cell>(argument)->GetFirst();
}
