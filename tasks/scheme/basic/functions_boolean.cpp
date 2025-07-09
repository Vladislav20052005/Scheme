#include "functions.h"

std::shared_ptr<Object> IsBooleanFunction::operator()(std::shared_ptr<Object> argument) {
    if (!Is<Cell>(argument)) {
        return std::make_shared<Symbol>("#f");
    }
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());
    if (!Is<Symbol>(computed_arg)) {
        return std::make_shared<Symbol>("#f");
    }
    std::string str = As<Symbol>(computed_arg)->GetName();
    return ToBoolean(str == "#t" || str == "#f");
}

std::shared_ptr<Object> NotFunction::operator()(std::shared_ptr<Object> argument) {
    if (!Is<Cell>(argument)) {
        throw RuntimeError("Not 1 arg");
    }
    if (As<Cell>(argument)->GetSecond()) {
        throw RuntimeError("Not 1 arg");
    }
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());

    if (!Is<Symbol>(computed_arg)) {
        return std::make_shared<Symbol>("#f");
    }
    return ToBoolean(As<Symbol>(computed_arg)->GetName() == "#f");
}

std::shared_ptr<Object> AndFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (Is<Symbol>(computed_arg) && As<Symbol>(computed_arg)->GetName() == "#f") {
            return ToBoolean(false);
        }
    }
    if (args.empty()) {
        return ToBoolean(true);
    }
    return Compute(args.back());
}

std::shared_ptr<Object> OrFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Symbol>(computed_arg) || As<Symbol>(computed_arg)->GetName() == "#t") {
            return computed_arg;
        }
    }
    return ToBoolean(false);
}
