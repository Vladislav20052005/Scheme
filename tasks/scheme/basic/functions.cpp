#include "functions.h"

AbstractFunction* GetFunction(
    std::shared_ptr<Object> obj) {  // must construct functors with pointers to variable buffers
    if (!Is<Symbol>(obj)) {
        throw RuntimeError("GetFunction got non-symbol value");
    }
    std::string name = As<Symbol>(obj)->GetName();

    if (name == "quote") {
        return new QuoteFunction();
    }
    if (name == "=") {
        return new CompareFunction([](long long a, long long b) { return (a == b); });
    }
    if (name == "<") {
        return new CompareFunction([](long long a, long long b) { return (a < b); });
    }
    if (name == ">") {
        return new CompareFunction([](long long a, long long b) { return (a > b); });
    }
    if (name == "<=") {
        return new CompareFunction([](long long a, long long b) { return (a <= b); });
    }
    if (name == ">=") {
        return new CompareFunction([](long long a, long long b) { return (a >= b); });
    }

    if (name == "number?") {
        return new IsNumberFunction();
    }
    if (name == "+") {
        return new AddFunction();
    }
    if (name == "-") {
        return new SubFunction();
    }
    if (name == "*") {
        return new MulFunction();
    }
    if (name == "/") {
        return new DivFunction();
    }

    if (name == "max") {
        return new MaxFunction();
    }
    if (name == "min") {
        return new MinFunction();
    }
    if (name == "abs") {
        return new AbsFunction();
    }

    if (name == "boolean?") {
        return new IsBooleanFunction();
    }
    if (name == "not") {
        return new NotFunction();
    }
    if (name == "and") {
        return new AndFunction();
    }
    if (name == "or") {
        return new OrFunction();
    }

    if (name == "pair?") {
        return new IsPairFunction();
    }
    if (name == "null?") {
        return new IsNullFunction();
    }
    if (name == "list?") {
        return new IsListFunction();
    }
    if (name == "cons") {
        return new ConsFunction();
    }
    if (name == "car") {
        return new CarFunction();
    }
    if (name == "cdr") {
        return new CdrFunction();
    }
    if (name == "list") {
        return new ListFunction();
    }
    if (name == "list-ref") {
        return new ListRefFunction();
    }
    if (name == "list-tail") {
        return new ListTailFunction();
    }
    throw RuntimeError("GetFunction : Incorrect function");
}

std::shared_ptr<Object> QuoteFunction::operator()(std::shared_ptr<Object> argument) {
    return As<Cell>(argument)->GetFirst();
}
