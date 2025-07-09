#pragma once

#include <string>
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include "error.h"
#include "scheme.h"
#include "scheme_memory.h"
#include <vector>
#include <sstream>
#include <iostream>
#include "helpers.h"

class Variable;

struct AbstractFunction : Object {
    virtual ~AbstractFunction() = default;
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) {
        return argument;
    }
};

std::shared_ptr<AbstractFunction> GetFunction(std::shared_ptr<Object> obj);

bool IsFunction(std::string name);

struct QuoteFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct IfFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct IsSymbolFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct DefineFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct SetFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct SetCarFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct SetCdrFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

std::shared_ptr<Object> MakeLambda(std::shared_ptr<Object> args,
                                   std::shared_ptr<Object> instructions);

struct LambdaFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct LambdaRun : AbstractFunction {
    LambdaRun(std::vector<std::shared_ptr<Object>>&& lambda_params,
              std::vector<std::shared_ptr<Object>>&& lambda_instructions,
              std::vector<std::shared_ptr<Variable>>&& context)
        : lambda_params_(std::move(lambda_params)),
          lambda_instructions_(std::move(lambda_instructions)),
          context_(std::move(context)) {
    }
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;

    std::vector<std::shared_ptr<Object>> lambda_params_;
    std::vector<std::shared_ptr<Object>> lambda_instructions_;
    std::vector<std::shared_ptr<Variable>> context_;
};

struct CompareFunction : AbstractFunction {
    CompareFunction() = delete;
    CompareFunction(bool (*compare)(long long, long long)) : compare_(compare) {
    }
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;

    bool (*compare_)(long long, long long);
};

struct IsNumberFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct DirectArithmeticFunction : AbstractFunction {
    DirectArithmeticFunction() = delete;
    DirectArithmeticFunction(long long (*func)(long long, long long), long long init)
        : func_(func), init_(init) {
    }
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;

    long long (*func_)(long long, long long);
    long long init_;
};

struct ReverseArithmeticFunction : AbstractFunction {
    ReverseArithmeticFunction() = delete;
    ReverseArithmeticFunction(long long (*func)(long long, long long), long long init)
        : func_(func), init_(init) {
    }
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;

    long long (*func_)(long long, long long);
    long long init_;
};

struct ExtremumFunction : AbstractFunction {
    ExtremumFunction() = delete;
    ExtremumFunction(long long (*func)(long long, long long)) : func_(func) {
    }
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;

    long long (*func_)(long long, long long);
};

struct AbsFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct IsBooleanFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct NotFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct AndFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct OrFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct IsPairFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct IsNullFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct IsListFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct ConsFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct CarFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct CdrFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct ListFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct ListRefFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct ListTailFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};
