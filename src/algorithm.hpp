#pragma once

#include "parser.hpp"
#include "table.hpp"

table::VarTable *vTable;

int literalToTypeId(parser::LiteralType literalT);

int determineExpressionType(parser::ExpressionNode *expNode);

int defOperation(parser::FuncDefNode *func_def_node,table::FuncTable *func_table,table::TypeTable *type_table,table::VarTable *var_table);
