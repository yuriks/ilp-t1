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

int determineExpressionType(parser::ExpressionNode *expNode, table::VarTable *vVTable, table::FuncTable *vFTable)
{
    if(expNode->type == parser::E_BINARY_OP 
        || expNode->type == parser::E_UNARY_OP 
        || expNode->type == parser::E_FUNCTION)
    {
        std::vector<int> vTypes;
        for(int i = 0 ; i < expNode->parameters.size() ; i++)
        {
            int p = determineExpressionType(&expNode->parameters[i],vVTable,vFTable);
            vTypes.push_back(p);
        }
        table::FuncEntry *vFres = vFTable->lookup(expNode->name,vTypes);

        if(vFres != nullptr)
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

void feedTables(parser::BaseNode *node, table::TypeTable *type_table, table::VarTable *var_table, table::FuncTable *func_table)
{
    if(node->type == parser::NODE_TYPE_DEF)
    {
        parser::TypeDefNode *f = (parser::TypeDefNode *)node;
        type_table->insert(f->type_name);
    }
    else if(node->type == parser::NODE_VAR_DEF)
    {
        parser::VarDefNode *f = (parser::VarDefNode *)node;
        int exp_type = determineExpressionType(&f->value,var_table,func_table);
        var_table->insert(f->var_name, exp_type);
 
    }
    else if(node->type == parser::NODE_FUNC_DEF)
    {
        parser::FuncDefNode *f = (parser::FuncDefNode *)node;
        func_table->insert(f->func_name,
        table::toTypeIds(type_table, f->param_types),
        table::toTypeId(type_table, f->return_type));
    }
}
