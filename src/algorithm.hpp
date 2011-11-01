#pragma once

#include "parser.hpp"
#include "table.hpp"

table::VarTable *vTable;

int literalToTypeId(parser::LiteralType literalT);

int determineExpressionType(parser::ExpressionNode *expNode);
 
