#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include "error.h"
#include "functions_declaration.h"
#include "garbage_collector.h"

class Collector;

class Interpreter {
public:
    std::string Run(const std::string&);
    ~Interpreter();
};

std::shared_ptr<Object> Compute(std::shared_ptr<Object> tree);
std::string Serialize(std::shared_ptr<Object> tree, bool ongoing = false);
