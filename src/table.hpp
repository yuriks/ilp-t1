#pragma once

#include <string>
#include <vector>
#include "parser.hpp"

namespace inference {

struct TypeEntry {
    std::string name;
    parser::LiteralType type;
};

struct SymbolTable {
    std::vector<TypeEntry> elements;

    void insert(const TypeEntry &reg);
    TypeEntry *lookup(const std::string &name);
};

void SymbolTable::insert(const TypeEntry &reg)
{
    TypeEntry *p = lookup(reg.name);
    if(p == NULL)
    {
        elements.push_back(reg);
    }
    else
    {
        //error. registry already defined.
    }
}

Registry *SymbolTable::lookup(const std::string &name)
{
    Registry *reg = NULL;

    for(std::vector<Registry>::iterator it = elements.begin(); it != elements.end(); it++)
    {
        if((*it).name.compare(name) == 0)
        {
            reg = &(*it);
            break;
        }
    }

    return reg;
}

} //namespace inference
