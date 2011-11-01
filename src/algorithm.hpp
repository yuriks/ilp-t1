#ifndef __ALGORITHM__HPP__
#define __ALGORITHM__HPP__

#include "parser.hpp"
#include "table.hpp"

namespace parser{

int literalToTypeId(LiteralType literalT)
{
    if(literalT == LIT_FLOAT)
    {
        return table::FLOAT;
    }
    if(literalT == LIT_INT)
    {
        return table::INT;
    }
    if(literalT == LIT_CHAR)
    {
        return table::CHAR;
    }
    if(literalT ==LIT_STRING)
    {
        return table::STRING;
    }
    if(literalT == LIT_BOOL)
    {
        return table::BOOL;
    }
    return -1;
}

}

namespace table {

static int const INT = 0;
static int const FLOAT = 1;
static int const CHAR = 2;
static int const BOOL = 3;
static int const STRING = 4;
static int const ERROR_TYPE = -1;

struct CompareElement
{
    int id_type_name1;
    int id_type_name2;
    int id_result;

    CompareElement(int t1, int t2, int tr)
    {
        id_type_name1 = t1;
        id_type_name2 = t2;
        id_result = tr;
    }
};

struct CompareTable {
    std::vector<CompareElement> elements;
    
    void insert(const int &typen1, const int &typen2, const int &typer)
    {
        elements.push_back(CompareElement(typen1,typen2,typer));
    }
    //se tiver o tipo retorna, se n retorna -1
    int lookup(const int &t1, const int &t2)
    {
        for(int i = 0; i < elements.size(); i++)
        {
            if(t1 == elements[i].id_type_name1 && t2 == elements[i].id_type_name2
                || t2 == elements[i].id_type_name1 && t1 == elements[i].id_type_name2){
                return elements[i].id_result;
            }
        }

        return ERROR_TYPE;
    }

    /*
     * int e int = int
     * int e float = float
     * float e float = float
     * char e char = char
     * bool e bool =  bool
     * string e string = string
     */
    void buildDefaultCompareTable()
    {
        elements.push_back(CompareElement(INT,INT,INT));
        elements.push_back(CompareElement(INT,FLOAT,FLOAT));
        elements.push_back(CompareElement(FLOAT,FLOAT,FLOAT));
        elements.push_back(CompareElement(CHAR,CHAR,CHAR));
        elements.push_back(CompareElement(BOOL,BOOL,BOOL));
        elements.push_back(CompareElement(STRING,STRING,STRING));
    }
    
};

}

table::VarTable *vTable;

int determineExpressionType(parser::ExpressionNode *expNode)
{
    if(expNode->type == parser::E_BINARY_OP 
        || expNode->type == parser::E_UNARY_OP 
        || expNode->type == parser::E_FUNCTION)
    {

    }else if(expNode->type == parser::E_VARIABLE)
    {
        //verifica o "name" na tabela de variáveis e pega o tipo
        //retorna o tipo
    }else if(expNode->type == parser::E_LITERAL)
    {
        return parser::literalToTypeId(expNode->literal_type);
    }
 
}




#endif