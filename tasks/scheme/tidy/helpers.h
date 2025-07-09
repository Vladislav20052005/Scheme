#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "object.h"
#include "error.h"
#include "functions_declaration.h"

std::vector<std::shared_ptr<Object>> AsVector(std::shared_ptr<Object> args);

std::shared_ptr<Symbol> ToBoolean(bool arg);
