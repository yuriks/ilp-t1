#pragma once

#include <string>
#include <vector>
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
    std::string func_name;
    std::vector<int> params_types_ids;
    int return_type_id;
};

struct VarEntry {
    std::string var_name;
    int type_id;
};

struct CompareEntry
{
    int id_type_name1;
    int id_type_name2;
    int id_result;

    CompareEntry(int t1, int t2, int tr);
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

    void insert(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id);
    FuncEntry *lookup(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id);
};

struct VarTable {
    std::vector<VarEntry> elements;

    void insert(const std::string &var_name, int type_id);
    VarEntry *lookup(const std::string &var_name);
};


struct CompareTable {
    std::vector<CompareEntry> elements;
    
    void insert(const int &typen1, const int &typen2, const int &typer);

    //se tiver o tipo retorna, se n retorna -1
    int lookup(const int &t1, const int &t2);

    /*
     * int e int = int
     * int e float = float
     * float e float = float
     * char e char = char
     * bool e bool =  bool
     * string e string = string
     */
    void buildDefaultCompareTable();
    
};


std::vector<int> toTypeIds(TypeTable *type_table, std::vector<std::string> type_list);
int toTypeId(TypeTable *type_table, std::string type_name);

} //namespace inference
