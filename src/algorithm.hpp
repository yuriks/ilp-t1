#pragma once

#include "parser.hpp"
#include "table.hpp"

int literalToTypeId(parser::LiteralType literalT);

int determineExpressionType(parser::ExpressionNode *expNode);
 
