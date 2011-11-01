#include "algorithm.hpp"
#include "table.hpp"

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

int defOperation(parser::FuncDefNode *func_def_node,table::FuncTable *func_table,table::TypeTable *type_table)
{
    func_table->insert(func_def_node->func_name,
        table::toTypeIds(type_table, func_def_node->param_types),
        table::toTypeId(type_table, func_def_node->return_type));
}
