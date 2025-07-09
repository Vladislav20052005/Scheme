#include "functions_declaration.h"

std::shared_ptr<Object> IsPairFunction::operator()(std::shared_ptr<Object> argument) {
    return ToBoolean(Is<Cell>(Compute(As<Cell>(argument)->GetFirst())));
}

std::shared_ptr<Object> IsNullFunction::operator()(std::shared_ptr<Object> argument) {
    return ToBoolean(!Compute(As<Cell>(argument)->GetFirst()));
}

std::shared_ptr<Object> IsListFunction::operator()(std::shared_ptr<Object> argument) {
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());
    try {
        AsVector(computed_arg);
    } catch (RuntimeError& re) {
        return ToBoolean(false);
    }
    return ToBoolean(true);
}

std::shared_ptr<Object> ConsFunction::operator()(std::shared_ptr<Object> argument) {
    return std::make_shared<Cell>(Compute(As<Cell>(argument)->GetFirst()),
                                  Compute(As<Cell>(As<Cell>(argument)->GetSecond())->GetFirst()));
}

std::shared_ptr<Object> CarFunction::operator()(std::shared_ptr<Object> argument) {
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());
    if (!computed_arg) {
        throw RuntimeError("null arg in car");
    }
    return As<Cell>(computed_arg)->GetFirst();
}

std::shared_ptr<Object> CdrFunction::operator()(std::shared_ptr<Object> argument) {
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());
    if (!computed_arg) {
        throw RuntimeError("null arg in cdr");
    }
    return As<Cell>(computed_arg)->GetSecond();
}

std::shared_ptr<Object> ListFunction::operator()(std::shared_ptr<Object> argument) {
    return argument;
}

std::shared_ptr<Object> ListRefFunction::operator()(std::shared_ptr<Object> argument) {
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());
    auto list = AsVector(computed_arg);
    long long i = As<Number>(As<Cell>(As<Cell>(argument)->GetSecond())->GetFirst())->GetValue();
    if (i < 0 || i >= static_cast<long long>(list.size())) {
        throw RuntimeError("Incorrect index in list-ref");
    }
    return list[i];
}

std::shared_ptr<Object> ListTailFunction::operator()(std::shared_ptr<Object> argument) {
    long long i = As<Number>(As<Cell>(As<Cell>(argument)->GetSecond())->GetFirst())->GetValue();
    if (i < 0) {
        throw RuntimeError("Incorrect index in list-tail");
    }
    auto computed_arg = Compute(As<Cell>(argument)->GetFirst());
    std::shared_ptr<Object> current = computed_arg;

    while (Is<Cell>(current) && i > 0) {
        current = As<Cell>(current)->GetSecond();
        i--;
    }
    if (i == 0) {
        return current;
    }
    throw RuntimeError("Incorrect index in list-tail");
}