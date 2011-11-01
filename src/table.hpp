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

    void insert(const TypeEntry &type_entry);
    TypeEntry *lookup(const std::string &name);
};

void SymbolTable::insert(const TypeEntry &type_entry)
{
    TypeEntry *p = lookup(type_entry.name);
    if(p == nullptr)
    {
        elements.push_back(type_entry);
    }
    else
    {
        //error. TypeEntry already defined.
    }
}

TypeEntry *SymbolTable::lookup(const std::string &name)
{
    TypeEntry *type_entry = nullptr;

    for(std::vector<TypeEntry>::iterator it = elements.begin(); it != elements.end(); it++)
    {
        if((*it).name.compare(name) == 0)
        {
            type_entry = &(*it);
            break;
        }
    }

    return type_entry;
}

} //namespace inference
