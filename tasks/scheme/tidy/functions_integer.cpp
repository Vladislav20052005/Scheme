#include "functions_declaration.h"

std::shared_ptr<Object> CompareFunction::operator()(std::shared_ptr<Object> argument) {
    if (!argument) {
        return std::make_shared<Symbol>("#t");
    }
    auto args = AsVector(argument);
    for (size_t i = 0; i < args.size() - 1; i++) {
        auto computed_argf = Compute(args[i]);
        auto computed_args = Compute(args[i + 1]);
        if (!Is<Number>(computed_argf) || !Is<Number>(computed_args)) {
            throw RuntimeError("Comparation argument is not a number : " +
                               Serialize(computed_argf));
        }
        if (!compare_(As<Number>(computed_argf)->GetValue(),
                      As<Number>(computed_args)->GetValue())) {
            return std::make_shared<Symbol>("#f");
        }
    }
    return std::make_shared<Symbol>("#t");
}

std::shared_ptr<Object> IsNumberFunction::operator()(std::shared_ptr<Object> argument) {
    return ToBoolean(Is<Number>(Compute(As<Cell>(argument)->GetFirst())));
}

std::shared_ptr<Object> DirectArithmeticFunction::operator()(std::shared_ptr<Object> argument) {
    long long result = init_;
    auto args = AsVector(argument);
    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Add/Mul argument is not a number");
        }
        result = func_(result, As<Number>(computed_arg)->GetValue());
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> ReverseArithmeticFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.empty()) {
        throw RuntimeError("Sub/Div has no arguments");
    }
    auto first_computed = Compute(args[0]);
    if (!Is<Number>(first_computed)) {
        throw RuntimeError("Sub/Div argument is not a number");
    }
    long long result = As<Number>(first_computed)->GetValue();

    for (auto arg_it = ++args.begin(); arg_it != args.end(); ++arg_it) {
        auto computed_arg = Compute(*arg_it);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Sub/Div argument is not a number");
        }
        if (init_ == 1 && As<Number>(computed_arg)->GetValue() == 0) {
            throw RuntimeError("Div argument is zero");
        }
        result = func_(result, As<Number>(computed_arg)->GetValue());
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> ExtremumFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.empty()) {
        throw RuntimeError("No arguments in max/min");
    }
    if (!Is<Number>(Compute(args[0]))) {
        throw RuntimeError("Max/Min argument is not a number");
    }
    for (auto& arg : args) {
        arg = Compute(arg);
        if (!Is<Number>(arg)) {
            throw RuntimeError("Max/Min argument is not a number");
        }
    }

    long long extr = As<Number>(args[0])->GetValue();

    for (auto arg : args) {
        extr = func_(extr, As<Number>(arg)->GetValue());
    }
    return std::make_shared<Number>(extr);
}

std::shared_ptr<Object> AbsFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.size() != 1) {
        throw RuntimeError("Not one arguments in abs");
    }
    if (!Is<Number>(Compute(args[0]))) {
        throw RuntimeError("Abs argument is not a number");
    }
    long long abs = std::abs(As<Number>(Compute(args[0]))->GetValue());

    return std::make_shared<Number>(abs);
}