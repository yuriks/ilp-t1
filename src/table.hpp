#pragma once

#include <string>
#include <vector>
#include "parser.hpp"

namespace inference {

/* Type of Entries */
struct TypeEntry {
    std::string type_name;
    int id;
};

struct FuncEntry {
    std::string func_name;
    std::vector<int> params_types_ids;
    int return_type_id;
};

struct VarEntry {
    std::string var_name;
    int type_id;
};

/*   Tables    */
struct TypeTable {
    std::vector<TypeEntry> elements;

    void insert(const std::string &type_name);
    TypeEntry *lookup(const std::string &type_name);
    TypeEntry *lookup(int type_id);
};

struct FuncTable {
    std::vector<FuncEntry> elements;

    void insert(const std::string &func_name, std::vector<std::string> params_types, std::string return_type);
    FuncEntry *lookup(const std::string &func_name, std::vector<std::string> params_types, std::string return_type);
};

struct VarTable {
    std::vector<VarEntry> elements;

    void insert(const std::string &var_name, const std::string &type);
    VarEntry *lookup(const std::string &var_name);
};

} //namespace inference
