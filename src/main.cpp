#include "parser.hpp"
#include "table.hpp"
#include "algorithm.hpp"

#include <cstdlib>
#include <iostream>
#include <string>




int main()
{
    table::TypeTable type_table;
    table::FuncTable func_table;
    table::VarTable var_table;
    
    type_table.insert("int");
    type_table.insert("float");
    type_table.insert("char");
    type_table.insert("bool");
    type_table.insert("string");

    int int_id = table::toTypeId(&type_table,"int");
    int float_id = table::toTypeId(&type_table,"float");

    std::vector<int> params;
    params.push_back(int_id);
    params.push_back(int_id);
    func_table.insert("+", params, int_id);
    params.clear();
    params.push_back(int_id);
    params.push_back(float_id);
    func_table.insert("+",params,float_id);

    parser::VarDefNode v;
    v.type = parser::NODE_VAR_DEF;
    v.var_name = "x";
    
    parser::ExpressionNode e;
    e.type = parser::E_FUNCTION;
    e.name = "+";
    
    std::vector<parser::ExpressionNode> ps;
    parser::ExpressionNode e1;
    e1.type = parser::E_LITERAL;
    e1.literal_type = parser::LIT_INT;
    
    parser::ExpressionNode e2;
    e2.type = parser::E_LITERAL;
    e2.literal_type = parser::LIT_INT;

    ps.push_back(e1);
    ps.push_back(e2);

    e.parameters = ps;
    v.value = e;

    feedTables(&v,&type_table,&var_table,&func_table);
    var_table.print(&type_table);

    system("pause");

    return 0;
}