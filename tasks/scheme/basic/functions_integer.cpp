#include "functions.h"

std::shared_ptr<Object> CompareFunction::operator()(std::shared_ptr<Object> argument) {
    if (!argument) {
        return std::make_shared<Symbol>("#t");
    }
    auto args = AsVector(argument);
    for (size_t i = 0; i < args.size() - 1; i++) {
        auto computed_argf = Compute(args[i]);
        auto computed_args = Compute(args[i + 1]);
        if (!Is<Number>(computed_argf) || !Is<Number>(computed_args)) {
            throw RuntimeError("Comparation argument is not a number");
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

std::shared_ptr<Object> AddFunction::operator()(std::shared_ptr<Object> argument) {
    long long sum = 0;
    auto args = AsVector(argument);
    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Add argument is not a number");
        }
        sum += As<Number>(computed_arg)->GetValue();
    }
    return std::make_shared<Number>(sum);
}

std::shared_ptr<Object> SubFunction::operator()(std::shared_ptr<Object> argument) {
    long long sub = 0;
    auto args = AsVector(argument);
    if (args.empty()) {
        throw RuntimeError("Sub has no arguments");
    }
    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Sub argument is not a number");
        }
        sub -= As<Number>(computed_arg)->GetValue();
    }
    sub += 2LL * As<Number>(Compute(args[0]))->GetValue();
    return std::make_shared<Number>(sub);
}

std::shared_ptr<Object> MulFunction::operator()(std::shared_ptr<Object> argument) {
    long long mul = 1;
    auto args = AsVector(argument);
    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Mul argument is not a number");
        }
        mul *= As<Number>(computed_arg)->GetValue();
    }
    return std::make_shared<Number>(mul);
}

std::shared_ptr<Object> DivFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.empty()) {
        throw RuntimeError("Div has no arguments");
    }
    long long div = As<Number>(Compute(args[0]))->GetValue();

    for (auto arg_it = ++args.begin(); arg_it != args.end(); ++arg_it) {
        auto computed_arg = Compute(*arg_it);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Div argument is not a number");
        }
        if (As<Number>(computed_arg)->GetValue() == 0) {
            throw RuntimeError("Div argument is zero");
        }
        div /= As<Number>(computed_arg)->GetValue();
    }
    return std::make_shared<Number>(div);
}

std::shared_ptr<Object> MaxFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.empty()) {
        throw RuntimeError("No arguments in max");
    }
    if (!Is<Number>(Compute(args[0]))) {
        throw RuntimeError("Max argument is not a number");
    }
    long long max = As<Number>(Compute(args[0]))->GetValue();

    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Max argument is not a number");
        }
        max = std::max(max, As<Number>(computed_arg)->GetValue());
    }
    return std::make_shared<Number>(max);
}

std::shared_ptr<Object> MinFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.empty()) {
        throw RuntimeError("No arguments in min");
    }
    if (!Is<Number>(Compute(args[0]))) {
        throw RuntimeError("Min argument is not a number");
    }
    long long min = As<Number>(Compute(args[0]))->GetValue();

    for (auto arg : args) {
        auto computed_arg = Compute(arg);
        if (!Is<Number>(computed_arg)) {
            throw RuntimeError("Min argument is not a number");
        }
        min = std::min(min, As<Number>(computed_arg)->GetValue());
    }
    return std::make_shared<Number>(min);
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