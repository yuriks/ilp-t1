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

    void insert(const std::string &type_name);
    TypeEntry *lookup(const std::string &type_name);
    TypeEntry *lookup(int type_id);

    void print();
};

struct FuncTable {
    std::map<std::string,std::vector<FuncEntry>> map_elements;

    void insert(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id);
    bool lookup(const std::string &func_name, std::vector<int> &params_types_ids, FuncEntry &func_entry);

    void print(TypeTable *type_table);
};

struct VarTable {
    std::vector<VarEntry> elements;

    void insert(const std::string &var_name, int type_id);
    VarEntry *lookup(const std::string &var_name);

    //for debug
    void print(TypeTable *type_table);
};

std::vector<int> toTypeIds(TypeTable *type_table, std::vector<std::string> type_list);
int toTypeId(TypeTable *type_table, std::string type_name);
std::string toStringFromTypeId(TypeTable *type_table, int type_id);


} //namespace inference
