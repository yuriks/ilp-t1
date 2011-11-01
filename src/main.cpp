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
