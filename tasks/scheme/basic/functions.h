#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include "error.h"
#include "scheme.h"
#include "helpers.h"

struct AbstractFunction {
    virtual ~AbstractFunction() = default;
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) {
        return argument;
    }
};

AbstractFunction* GetFunction(std::shared_ptr<Object> obj);

struct QuoteFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
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

struct AddFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct SubFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct MulFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct DivFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct MaxFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
};

struct MinFunction : AbstractFunction {
    std::shared_ptr<Object> operator()(std::shared_ptr<Object> argument) override;
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
