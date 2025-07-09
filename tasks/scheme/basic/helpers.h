#pragma once

#include "object.h"
#include "error.h"
#include <vector>
#include <string>
#include <memory>

std::vector<std::shared_ptr<Object>> AsVector(std::shared_ptr<Object> args);

std::shared_ptr<Symbol> ToBoolean(bool arg);
