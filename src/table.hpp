#pragma once

#include <string>
#include <vector>
#include <map>

namespace table {

/* Type of Entries */
struct TypeEntry {
    std::string type_name;
    int type_id;
};

struct FuncEntry {
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
    std::map<std::string,std::vector<FuncEntry>> map_elements;

    void insert(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id);
    FuncEntry *lookup(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id);
};

struct VarTable {
    std::vector<VarEntry> elements;

    void insert(const std::string &var_name, int type_id);
    VarEntry *lookup(const std::string &var_name);
};

} //namespace inference
