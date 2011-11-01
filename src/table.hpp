#pragma once

#include <string>
#include <vector>
#include "parser.hpp"

namespace inference {

struct Registry {
    std::string name;
    parser::LiteralType type;
};

struct SymbolTable {
    std::vector<Registry> elements;
};

} //namespace inference
