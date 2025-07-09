#include "functions_declaration.h"

std::shared_ptr<Object> IfFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (2 > args.size() || args.size() > 3) {
        throw SyntaxError("Incorrect number of args in if");
    }
    auto condition = Compute(args[0]);
    if (!Is<Symbol>(condition) || As<Symbol>(condition)->GetName() == "#t") {
        return Compute(args[1]);
    } else {
        if (args.size() == 2) {
            return nullptr;
        } else {
            return Compute(args[2]);
        }
    }
}

std::shared_ptr<Object> IsSymbolFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.size() != 1) {
        throw SyntaxError("Incorrect number of args in symbol?");
    }
    return ToBoolean(Is<Symbol>(Compute(args[0])));
}

std::shared_ptr<Object> DefineFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.empty()) {
        throw SyntaxError("Incorrect number of args in define");
    }
    if (Is<Cell>(args[0])) {
        auto new_lambda =
            MakeLambda(As<Cell>(args[0])->GetSecond(), As<Cell>(argument)->GetSecond());
        if (!Is<Symbol>(As<Cell>(args[0])->GetFirst())) {
            throw SyntaxError("Lambda name is not a symbol");
        }
        As<LambdaRun>(new_lambda)
            ->context_.push_back(std::make_shared<Variable>(
                As<Symbol>(As<Cell>(args[0])->GetFirst())->GetName(), new_lambda));
        GetMemory().DefineVariable(As<Symbol>(As<Cell>(args[0])->GetFirst())->GetName(),
                                   new_lambda);
        return nullptr;
    }
    if (args.size() != 2) {
        throw SyntaxError("Incorrect number of args in define");
    }
    if (!Is<Symbol>(args[0])) {
        throw SyntaxError("Incorrect type of variable name in define");
    }
    auto computed_arg = Compute(args[1]);
    if (Is<LambdaRun>(computed_arg)) {
        As<LambdaRun>(computed_arg)
            ->context_.push_back(
                std::make_shared<Variable>(As<Symbol>(args[0])->GetName(), computed_arg));
    }
    GetMemory().DefineVariable(As<Symbol>(args[0])->GetName(), computed_arg);
    return nullptr;
}

std::shared_ptr<Object> SetFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.size() != 2) {
        throw SyntaxError("Incorrect number of args in set!");
    }
    if (!Is<Symbol>(args[0])) {
        throw SyntaxError("Incorrect type of variable name in set!");
    }
    auto computed_arg = Compute(args[1]);
    GetMemory().SetVariable(As<Symbol>(args[0])->GetName(), computed_arg);
    return nullptr;
}

std::shared_ptr<Object> SetCarFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.size() != 2) {
        throw SyntaxError("Incorrect number of args in set-car!");
    }
    auto computed_arg = Compute(args[1]);
    auto value = Compute(args[0]);
    if (!Is<Cell>(value)) {
        throw SyntaxError("Variable's value is not a complex structure");
    }
    As<Cell>(value)->SetFirst(computed_arg);
    return nullptr;
}

std::shared_ptr<Object> SetCdrFunction::operator()(std::shared_ptr<Object> argument) {
    auto args = AsVector(argument);
    if (args.size() != 2) {
        throw SyntaxError("Incorrect number of args in set-cdr!");
    }
    auto computed_arg = Compute(args[1]);
    auto value = Compute(args[0]);
    if (!Is<Cell>(value)) {
        throw SyntaxError("Variable's value is not a complex structure");
    }
    As<Cell>(value)->SetSecond(computed_arg);
    return nullptr;
}

std::shared_ptr<Object> LambdaFunction::operator()(std::shared_ptr<Object> argument) {
    std::vector<std::shared_ptr<Object>> args_vector = AsVector(argument);
    if (args_vector.size() < 2) {
        throw SyntaxError("Wrong lambda syntax");
    }
    return MakeLambda(As<Cell>(argument)->GetFirst(), As<Cell>(argument)->GetSecond());
}

std::shared_ptr<Object> MakeLambda(std::shared_ptr<Object> args,
                                   std::shared_ptr<Object> instructions) {
    std::vector<std::shared_ptr<Object>> lambda_params = AsVector(args);
    std::vector<std::shared_ptr<Object>> lambda_instructions = AsVector(instructions);
    std::vector<std::shared_ptr<Variable>> context = GetMemory().GetContext();
    return std::make_shared<LambdaRun>(std::move(lambda_params), std::move(lambda_instructions),
                                       std::move(context));
}

std::shared_ptr<Object> LambdaRun::operator()(std::shared_ptr<Object> argument) {
    std::vector<std::shared_ptr<Object>> lambda_args = AsVector(argument);
    if (lambda_args.size() != lambda_params_.size()) {
        throw RuntimeError("Incorrect number of arguments for lambda");
    }
    for (size_t i = 0; i < lambda_args.size(); i++) {
        lambda_args[i] = Compute(lambda_args[i]);
    }
    GetMemory().SetEndOfScope();
    for (size_t i = 0; i < context_.size(); i++) {
        GetMemory().DefineVariable(context_[i]);
    }

    for (size_t i = 0; i < lambda_params_.size(); i++) {
        GetMemory().DefineVariable(As<Symbol>(lambda_params_[i])->GetName(), lambda_args[i]);
    }
    std::shared_ptr<Object> computed_arg;
    for (size_t i = 0; i < lambda_instructions_.size(); i++) {
        computed_arg = Compute(lambda_instructions_[i]);
    }
    GetMemory().Pop();
    return computed_arg;
}