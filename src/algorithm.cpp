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

int determineExpressionType(const parser::ExpressionNode& expNode, const table::VarTable& vVTable, const table::FuncTable& vFTable, const table::TypeTable& type_table)
{
    if (expNode.type == parser::E_BINARY_OP
        || expNode.type == parser::E_UNARY_OP
        || expNode.type == parser::E_FUNCTION)
    {
        std::vector<int> vTypes;
        for (unsigned int i = 0 ; i < expNode.parameters.size() ; i++) {
            int p = determineExpressionType(expNode.parameters[i], vVTable, vFTable, type_table);
            vTypes.push_back(p);
        }

        table::FuncEntry vFres;
        if (vFTable.lookup(expNode.name, vTypes, vFres)) {
            return vFres.return_type_id;
        } else {
            std::string err("No function overload for ");
            err.append(expNode.name);
            err.append(" found matching operands: ");
            for (unsigned int i = 0; i < vTypes.size(); ++i) {
                if (i != 0)
                    err.append(", ");
                err.append(table::toStringFromTypeId(type_table, vTypes[i]));
            }
            throw std::runtime_error(err);
        }
    } else if (expNode.type == parser::E_VARIABLE) {
        const table::VarEntry* vres = vVTable.lookup(expNode.name);
        if (vres) {
            return vres->type_id;
        } else {
            return -1;
        }
    } else if (expNode.type == parser::E_LITERAL) {
        return literalToTypeId(expNode.literal_type);
    } else {
        return -1;
    }
}

void feedTables(const parser::BaseNode* node, table::TypeTable& type_table, table::VarTable& var_table, table::FuncTable& func_table)
{
    if (node->type == parser::NODE_TYPE_DEF) {
        auto f = static_cast<const parser::TypeDefNode*>(node);
        type_table.insert(f->type_name);
    } else if (node->type == parser::NODE_VAR_DEF) {
        auto f = static_cast<const parser::VarDefNode*>(node);
        int exp_type = determineExpressionType(f->value, var_table, func_table, type_table);
        var_table.insert(f->var_name, exp_type);
    } else if (node->type == parser::NODE_FUNC_DEF) {
        auto f = static_cast<const parser::FuncDefNode*>(node);
        func_table.insert(f->func_name,
            table::toTypeIds(type_table, f->param_types),
            table::toTypeId(type_table, f->return_type));
    }
}
