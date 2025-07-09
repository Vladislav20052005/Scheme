#include "helpers.h"

std::vector<std::shared_ptr<Object>> AsVector(std::shared_ptr<Object> args) {
    std::vector<std::shared_ptr<Object>> return_vector;
    std::shared_ptr<Object> current = args;
    while (Is<Cell>(current)) {
        return_vector.push_back(As<Cell>(current)->GetFirst());
        current = As<Cell>(current)->GetSecond();
    }
    if (current) {
        throw RuntimeError("AsVector got incorrect list / pair");
    }
    return return_vector;
}

std::shared_ptr<Symbol> ToBoolean(bool arg) {
    return arg ? std::make_shared<Symbol>("#t") : std::make_shared<Symbol>("#f");
}
