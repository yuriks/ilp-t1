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

int determineExpressionType(parser::ExpressionNode *expNode, table::VarTable *vVTable, table::FuncTable *vFTable)
{
    if(expNode->type == parser::E_BINARY_OP 
        || expNode->type == parser::E_UNARY_OP 
        || expNode->type == parser::E_FUNCTION)
    {
        std::vector<int> vTypes;
        for(int i = 0 ; i < expNode->parameters.size() ; i++)
        {

            vTypes.push_back(determineExpressionType(&expNode->parameters[i],vVTable,vFTable));
        }
        table::FuncEntry *vFres = vFTable->lookup(expNode->name,vTypes,expNode->type);
        if(vFres)
        {
            return vFres->return_type_id;
        }else{
            return -1;
        }
    }else if(expNode->type == parser::E_VARIABLE)
    {
        table::VarEntry* vres = vVTable->lookup(expNode->name);
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