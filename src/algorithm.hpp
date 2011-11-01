#pragma once

#include "parser.hpp"
#include "table.hpp"

int literalToTypeId(parser::LiteralType literalT);

int determineExpressionType(parser::ExpressionNode *expNode);

void defFuncOperation(parser::FuncDefNode *func_def_node,table::FuncTable *func_table,table::TypeTable *type_table,table::VarTable *var_table);
void defVarOperation(parser::VarDefNode *var_def_node,table::VarTable *var_table,table::TypeTable *type_table,table::FuncTable *func_table);