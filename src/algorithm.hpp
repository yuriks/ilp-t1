#pragma once

#include "parser.hpp"
#include "table.hpp"

int literalToTypeId(parser::LiteralType literalT);

int determineExpressionType(parser::ExpressionNode *expNode);

void feedTables(parser::BaseNode *node, table::TypeTable *type_table, table::VarTable *var_table, table::FuncTable *func_table);