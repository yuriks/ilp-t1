#include "algorithm.hpp"

int literalToTypeId(parser::LiteralType literalT)
{
    if(literalT == parser::LIT_FLOAT)
    {
        return table::FLOAT;
    }
    if(literalT == parser::LIT_INT)
    {
        return table::INT;
    }
    if(literalT == parser::LIT_CHAR)
    {
        return table::CHAR;
    }
    if(literalT == parser::LIT_STRING)
    {
        return table::STRING;
    }
    if(literalT == parser::LIT_BOOL)
    {
        return table::BOOL;
    }
    return -1;
}

int determineExpressionType(parser::ExpressionNode *expNode)
{
    if(expNode->type == parser::E_BINARY_OP 
        || expNode->type == parser::E_UNARY_OP 
        || expNode->type == parser::E_FUNCTION)
    {

    }else if(expNode->type == parser::E_VARIABLE)
    {
        table::VarEntry* vres = vTable->lookup(expNode->name);
        if(vres)
        {
            return vres->type_id;
        }else{
            return -1;
        }
    }else if(expNode->type == parser::E_LITERAL)
    {
        return literalToTypeId(expNode->literal_type);
    }
 
}