#pragma once

#include <string>
#include <vector>
#include <map>
#include "parser.hpp"

namespace table {

static int const INT = 0;
static int const FLOAT = 1;
static int const CHAR = 2;
static int const BOOL = 3;
static int const STRING = 4;
static int const ERROR_TYPE = -1;

/* Type of Entries */
struct TypeEntry {
    std::string type_name;
    int id;
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

    void insert(const std::string& type_name);
    const TypeEntry* lookup(const std::string& type_name) const;
    const TypeEntry* lookup(int type_id) const;

    void print() const;
};

struct FuncTable {
    std::map<std::string,std::vector<FuncEntry>> map_elements;

    void insert(const std::string& func_name, const std::vector<int>& params_types_ids, int return_type_id);
    bool lookup(const std::string& func_name, const std::vector<int>& params_types_ids, FuncEntry& func_entry) const;
    const FuncEntry* lookup(const std::string& func_name, const std::vector<int>& params_types_ids) const;

    void print(const TypeTable& type_table) const;
};

struct VarTable {
    std::vector<VarEntry> elements;

    void insert(const std::string& var_name, int type_id);
    const VarEntry* lookup(const std::string& var_name) const;

    //for debug
    void print(const TypeTable& type_table) const;
};

std::vector<int> toTypeIds(const TypeTable& type_table, const std::vector<std::string>& type_list);
int toTypeId(const TypeTable& type_table, const std::string& type_name);
std::string toStringFromTypeId(const TypeTable& type_table, int type_id);

} //namespace inference
