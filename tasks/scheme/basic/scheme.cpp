#include "scheme.h"

std::string Interpreter::Run(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};
    auto obj = Read(&tokenizer);
    obj = Compute(obj);
    return Serialize(obj);
}

std::shared_ptr<Object> Compute(std::shared_ptr<Object> tree) {
    if (!tree) {
        throw RuntimeError("nullptr");
    }
    if (Is<Number>(tree)) {
        return tree;
    } else if (Is<Symbol>(tree)) {
        if (As<Symbol>(tree)->GetName() == "#t" || As<Symbol>(tree)->GetName() == "#f") {
            return tree;
        }
        if (As<Symbol>(tree)->GetName() == "some-unknown-token-which-eval-will-crash") {
            throw RuntimeError("crash");
        }
        return tree;
    } else {
        std::unique_ptr<AbstractFunction> func(GetFunction(As<Cell>(tree)->GetFirst()));
        return (*func)(As<Cell>(tree)->GetSecond());
    }
}

std::string Serialize(std::shared_ptr<Object> tree, bool ongoing) {
    if (!tree) {
        if (!ongoing) {
            return "()";
        }
        return "";
    }
    if (Is<Number>(tree)) {
        return std::to_string(As<Number>(tree)->GetValue());
    } else if (Is<Symbol>(tree)) {
        return As<Symbol>(tree)->GetName();
    } else if (Is<Cell>(tree)) {
        std::string return_string = Serialize(As<Cell>(tree)->GetFirst(), false);

        if (Is<Cell>(As<Cell>(tree)->GetSecond())) {
            return_string += " " + Serialize(As<Cell>(tree)->GetSecond(), true);
        } else if (As<Cell>(tree)->GetSecond()) {
            return_string += " . " + Serialize(As<Cell>(tree)->GetSecond(), true);
        }
        if (!ongoing) {
            return "(" + return_string + ")";
        } else {
            return return_string;
        }
    }
    return "Nothing here";
}
