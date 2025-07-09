#include "scheme.h"



std::string Interpreter::Run(const std::string& str) {
    auto ptr = GetCollector().Make<Number>(12);
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};
    auto obj = Read(&tokenizer);
    obj = Compute(obj);
    return Serialize(obj);
}

Interpreter::~Interpreter() {
    GetMemory().Clear();
}

std::shared_ptr<Object> Compute(std::shared_ptr<Object> tree) {
    if (!tree) {
        throw RuntimeError("nullptr");
    }
    if (Is<Number>(tree)) {
        return tree;
    } else if (Is<Symbol>(tree)) {
        std::string name = As<Symbol>(tree)->GetName();
        if (name == "#t" || name == "#f") {
            return tree;
        }
        if (GetMemory().IsDefined(name)) {
            return GetMemory().GetVariable(name);
        }
        if (IsFunction(name)) {
            return std::shared_ptr<Object>(GetFunction(tree));
        }
        throw NameError("Wrong symbol name");
    } else {
        auto computed_func = Compute(As<Cell>(tree)->GetFirst());
        if (!Is<AbstractFunction>(computed_func)) {
            throw RuntimeError("First element in list does not evaluate to the functor");
        }
        std::shared_ptr<AbstractFunction> func(As<AbstractFunction>(computed_func));
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
    if (Is<LambdaRun>(tree)) {
        return "LAMBDA";
    }
    if (Is<AbstractFunction>(tree)) {
        return "Some non-LAMBDA function";
    }
    return "Nothing here";
}
