#include "scheme_memory.h"

Memory& GetMemory() {
    static Memory memory;
    return memory;
}

std::string Variable::GetName() {
    return name_;
}
std::shared_ptr<Object>& Variable::GetValue() {
    return value_;
}
void Variable::SetValue(std::shared_ptr<Object> value) {
    value_ = value;
}

void Memory::DefineVariable(const std::string& name, std::shared_ptr<Object> value) {
    storage_.push_back(std::make_shared<Variable>(name, value));
    Inc(name);
}

void Memory::DefineVariable(std::shared_ptr<Variable> variable) {
    storage_.push_back(variable);
    Inc(variable->GetName());
}

void Memory::SetEndOfScope() {
    storage_.push_back(std::make_shared<Variable>());
}

void Memory::SetVariable(const std::string& name, std::shared_ptr<Object> value) {
    for (int i = storage_.size() - 1; i != -1; i--) {
        if (storage_[i]->GetName() == name) {
            storage_[i]->GetValue() = value;
            return;
        }
    }
    throw NameError("Variable " + name + " is not defined");
}
void Memory::Print() {
    for (size_t i = 0; i < storage_.size(); i++) {
        std::cout << storage_[i]->GetName() << " ";
    }
    std::cout << "\n";
}
std::shared_ptr<Object> Memory::GetVariable(const std::string& name) {
    for (int i = storage_.size() - 1; i != -1; i--) {
        if (storage_[i]->GetName() == name) {
            return storage_[i]->GetValue();
        }
    }
    throw NameError("Variable " + name + " is not defined");
}

void Memory::Pop() {
    while (!IsEndOfScope(storage_.size() - 1)) {
        Dec(storage_.back()->GetName());
        storage_.pop_back();
    }
    if (!storage_.empty()) {
        Dec(storage_.back()->GetName());
        storage_.pop_back();
    }
}

std::vector<std::shared_ptr<Variable>> Memory::GetContext() {
    std::vector<std::shared_ptr<Variable>> return_vector;
    for (int i = storage_.size() - 1; !IsEndOfScope(i); i--) {
        return_vector.push_back(storage_[i]);
    }
    std::reverse(return_vector.begin(), return_vector.end());
    return return_vector;
}
bool Memory::IsDefined(const std::string& name) {
    return (defined_variables_counter_.count(name) != 0 && defined_variables_counter_[name] != 0);
}
void Memory::Clear() {
    storage_.clear();
    defined_variables_counter_.clear();
}

std::vector<std::shared_ptr<Variable>>& Memory::GetStorage() {
    return storage_;
}


bool Memory::IsEndOfScope(int index) {
    return (index == -1) || (storage_[index]->GetName().empty());
}

void Memory::Inc(const std::string& name) {
    if (!IsDefined(name)) {
        defined_variables_counter_[name] = 0;
    }
    defined_variables_counter_[name]++;
}

void Memory::Dec(const std::string& name) {
    if (!IsDefined(name)) {
        return;
    }
    if (defined_variables_counter_[name] == 0) {
        return;
    }
    defined_variables_counter_[name]--;
}