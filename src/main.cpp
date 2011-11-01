#include "parser.hpp"
#include "table.hpp"
#include "algorithm.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> makeVector(int a, int b)
{
    std::vector<int> v;
    v.push_back(a);
    v.push_back(b);
    return v;
}

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

    static const int INT_T = type_table.lookup("int")->id;
    static const int FLOAT_T = type_table.lookup("float")->id;
    static const int BOOL_T = type_table.lookup("bool")->id;
    
    func_table.insert("+", makeVector(0, 0), 0);
    func_table.insert("+", makeVector(0, 1), 1);
    func_table.insert("+", makeVector(1, 0), 1);
    func_table.insert("+", makeVector(1, 1), 1);
    func_table.insert("+", makeVector(0, 0), 0);

    func_table.insert("-", makeVector(0, 0), 0);
    func_table.insert("-", makeVector(0, 1), 1);
    func_table.insert("-", makeVector(1, 0), 1);
    func_table.insert("-", makeVector(1, 1), 1);
    func_table.insert("-", makeVector(0, 0), 0);

    func_table.insert("*", makeVector(0, 0), 0);
    func_table.insert("*", makeVector(0, 1), 1);
    func_table.insert("*", makeVector(1, 0), 1);
    func_table.insert("*", makeVector(1, 1), 1);
    func_table.insert("*", makeVector(0, 0), 0);

    func_table.insert("/", makeVector(0, 0), 0);
    func_table.insert("/", makeVector(0, 1), 1);
    func_table.insert("/", makeVector(1, 0), 1);
    func_table.insert("/", makeVector(1, 1), 1);
    func_table.insert("/", makeVector(0, 0), 0);

    func_table.insert("%", makeVector(0, 0), 0);
    func_table.insert("%", makeVector(0, 1), 1);
    func_table.insert("%", makeVector(1, 0), 1);
    func_table.insert("%", makeVector(1, 1), 1);
    func_table.insert("%", makeVector(0, 0), 0);

    func_table.insert("==", makeVector(0, 0), BOOL_T);
    func_table.insert("==", makeVector(0, 1), BOOL_T);
    func_table.insert("==", makeVector(1, 0), BOOL_T);
    func_table.insert("==", makeVector(1, 1), BOOL_T);
    func_table.insert("==", makeVector(0, 0), BOOL_T);

    func_table.insert("!=", makeVector(0, 0), BOOL_T);
    func_table.insert("!=", makeVector(0, 1), BOOL_T);
    func_table.insert("!=", makeVector(1, 0), BOOL_T);
    func_table.insert("!=", makeVector(1, 1), BOOL_T);
    func_table.insert("!=", makeVector(0, 0), BOOL_T);

    func_table.insert("<", makeVector(0, 0), BOOL_T);
    func_table.insert("<", makeVector(0, 1), BOOL_T);
    func_table.insert("<", makeVector(1, 0), BOOL_T);
    func_table.insert("<", makeVector(1, 1), BOOL_T);
    func_table.insert("<", makeVector(0, 0), BOOL_T);

    func_table.insert("<=", makeVector(0, 0), BOOL_T);
    func_table.insert("<=", makeVector(0, 1), BOOL_T);
    func_table.insert("<=", makeVector(1, 0), BOOL_T);
    func_table.insert("<=", makeVector(1, 1), BOOL_T);
    func_table.insert("<=", makeVector(0, 0),BOOL_T);

    func_table.insert(">", makeVector(0, 0), BOOL_T);
    func_table.insert(">", makeVector(0, 1), BOOL_T);
    func_table.insert(">", makeVector(1, 0), BOOL_T);
    func_table.insert(">", makeVector(1, 1), BOOL_T);
    func_table.insert(">", makeVector(0, 0), BOOL_T);

    func_table.insert(">=", makeVector(0, 1), BOOL_T);
    func_table.insert(">=", makeVector(0, 0), BOOL_T);
    func_table.insert(">=", makeVector(1, 0), BOOL_T);
    func_table.insert(">=", makeVector(1, 1), BOOL_T);
    func_table.insert(">=", makeVector(0, 0), BOOL_T);

    std::string line;
    std::vector<parser::TokenInfo> tokens;

    while (true) {
        std::cout << ">>> " << std::flush;
        std::getline(std::cin, line);
        if (line == "!printtypes") {
            type_table.print();
        } else if (line == "!printfuncs") {
            func_table.print(type_table);
        } else if (line == "!printvars") {
            var_table.print(type_table);
        } else if (line == "!quit") {
            break;
        } else {
            line.push_back('\n');
            while (!parser::tokenize(tokens, line)) {
                std::cout << "... " << std::flush;
                std::getline(std::cin, line);
                line.push_back('\n');
            }
            try {
                auto node = parser::parse(tokens);
                feedTables(node.get(), type_table, var_table, func_table);
            } catch (std::runtime_error& err) {
                std::cerr << err.what() << std::endl;
                tokens.clear();
                line.clear();
            }
        }
    }

    return 0;
}
