#pragma once

#include <string>
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include "error.h"
#include "functions.h"
#include <sstream>
#include <iostream>

class Interpreter {
public:
    std::string Run(const std::string&);
};

std::shared_ptr<Object> Compute(std::shared_ptr<Object> tree);
std::string Serialize(std::shared_ptr<Object> tree, bool ongoing = false);
