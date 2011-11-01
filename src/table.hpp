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

    void insert(const Registry &reg);
    Registry *lookup(const std::string &name);
};

void SymbolTable::insert(const Registry &reg)
{
    Registry *p = lookup(reg.name);
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
