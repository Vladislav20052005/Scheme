#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "object.h"
#include "error.h"
#include "scheme.h"
#include "functions_declaration.h"

class Variable {
public:
    Variable() : name_(), value_(nullptr) {
    }
    Variable(std::string name, std::shared_ptr<Object> value) : name_(name), value_(value) {
    }
    std::string GetName();
    std::shared_ptr<Object>& GetValue();
    void SetValue(std::shared_ptr<Object> value);

private:
    std::string name_;
    std::shared_ptr<Object> value_;
};

class Memory {
public:
    Memory() = default;
    void DefineVariable(const std::string& name, std::shared_ptr<Object> value);
    void DefineVariable(std::shared_ptr<Variable> variable);
    void Print();
    void SetEndOfScope();
    void SetVariable(const std::string& name, std::shared_ptr<Object> value);
    void Pop();
    std::shared_ptr<Object> GetVariable(const std::string& name);
    std::vector<std::shared_ptr<Variable>> GetContext();
    bool IsDefined(const std::string& name);
    void Clear();
    std::vector<std::shared_ptr<Variable>>& GetStorage();

private:
    bool IsEndOfScope(int index);
    void Inc(const std::string& name);
    void Dec(const std::string& name);

    std::vector<std::shared_ptr<Variable>> storage_;
    std::map<std::string, size_t> defined_variables_counter_;
};

Memory& GetMemory();
