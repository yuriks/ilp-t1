#pragma once

#include "parser.hpp"
#include "table.hpp"

void feedTables(const parser::BaseNode* node, table::TypeTable& type_table, table::VarTable& var_table, table::FuncTable& func_table);
