#include "functions_declaration.h"

static std::map<std::string, std::shared_ptr<AbstractFunction>> functions_list = {
    {"quote", std::make_shared<QuoteFunction>()},

    {"=", std::make_shared<CompareFunction>([](long long a, long long b) { return (a == b); })},
    {"<", std::make_shared<CompareFunction>([](long long a, long long b) { return (a < b); })},
    {">", std::make_shared<CompareFunction>([](long long a, long long b) { return (a > b); })},
    {"<=", std::make_shared<CompareFunction>([](long long a, long long b) { return (a <= b); })},
    {">=", std::make_shared<CompareFunction>([](long long a, long long b) { return (a >= b); })},

    {"number?", std::make_shared<IsNumberFunction>()},

    {"+",
     std::make_shared<DirectArithmeticFunction>([](long long a, long long b) { return a + b; }, 0)},
    {"-", std::make_shared<ReverseArithmeticFunction>(
              [](long long a, long long b) { return a - b; }, 0)},
    {"*",
     std::make_shared<DirectArithmeticFunction>([](long long a, long long b) { return a * b; }, 1)},
    {"/", std::make_shared<ReverseArithmeticFunction>(
              [](long long a, long long b) { return a / b; }, 1)},

    {"max",
     std::make_shared<ExtremumFunction>([](long long a, long long b) { return std::max(a, b); })},
    {"min",
     std::make_shared<ExtremumFunction>([](long long a, long long b) { return std::min(a, b); })},

    {"abs", std::make_shared<AbsFunction>()},

    {"boolean?", std::make_shared<IsBooleanFunction>()},

    {"not", std::make_shared<NotFunction>()},
    {"and", std::make_shared<AndFunction>()},
    {"or", std::make_shared<OrFunction>()},

    {"pair?", std::make_shared<IsPairFunction>()},
    {"null?", std::make_shared<IsNullFunction>()},
    {"list?", std::make_shared<IsListFunction>()},

    {"cons", std::make_shared<ConsFunction>()},
    {"car", std::make_shared<CarFunction>()},
    {"cdr", std::make_shared<CdrFunction>()},

    {"list", std::make_shared<ListFunction>()},
    {"list-ref", std::make_shared<ListRefFunction>()},
    {"list-tail", std::make_shared<ListTailFunction>()},

    {"if", std::make_shared<IfFunction>()},
    {"symbol?", std::make_shared<IsSymbolFunction>()},
    {"define", std::make_shared<DefineFunction>()},
    {"set!", std::make_shared<SetFunction>()},
    {"set-car!", std::make_shared<SetCarFunction>()},
    {"set-cdr!", std::make_shared<SetCdrFunction>()},
    {"lambda", std::make_shared<LambdaFunction>()}};

std::shared_ptr<AbstractFunction> GetFunction(std::shared_ptr<Object> obj) {
    return functions_list[As<Symbol>(obj)->GetName()];
}

bool IsFunction(std::string name) {
    return functions_list.count(name) != 0;
}

std::shared_ptr<Object> QuoteFunction::operator()(std::shared_ptr<Object> argument) {
    return As<Cell>(argument)->GetFirst();
}
