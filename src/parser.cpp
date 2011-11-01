/*
Language example:

class Foo;
class Bar;

def makeFoo() -> Foo;
def makeBar() -> Bar;

def fooFromBar(Bar) -> Foo;
def spam(Foo, Foo, Bar) -> Bar;

def barFromInt(int) -> Bar;

def operator +(int, int) -> int;

var x = makeFoo();
var y = x + fooFromBar(makeBar());
*/

/*
Grammar:

Identifier = [a-zA-Z_]+

Declaration = TypeDeclaration | FunctionDeclaration | VariableDeclaration
Statement = Declaration ';'

Type = Identifier
TypeDeclaration = 'class' Type

FunctionDeclaration = 'def' Identifier '(' ParameterListDecl '->' Type
ParameterListDecl = ')' | Type ')' | Type ',' ParameterList

VariableDeclaration = 'var' Identifier '=' Expression

FunctionCall = Identifier '(' ( ')' | ParameterListExpr )
ParameterListExpr = Expression ( ')' | ',' ParameterListExpr)

// Need to inline the '(' peek in FunctionCall I think
Tier0 = Literal | FunctionCall | Identifier | Expression
Tier1 = '!' Tier0 | '-' Tier0 | Tier0
Tier2 = Tier1 ('*' | '/' | '%') Tier1 | Tier1
Tier3 = Tier2 ('+' | '-') Tier2 | Tier2
Tier4 = Tier3 ('<' | '<=' | '>' | '>=') Tier3 | Tier3
Tier5 = Tier4 ('==' | '!=') Tier4 | Tier4
Tier6 = Tier5 '&&' Tier5 | Tier5
Tier7 = Tier6 '||' Tier6 | Tier6
Expression = '(' Tier7 ')' | Tier7


Operators:
1 ! -
2 * / %
3 + -
4 < <= > >=
5 == !=
6 &&
7 ||
*/

#include "parser.hpp"

#include <regex>
#include <sstream>

namespace {

static const std::regex token_re(
    "\\s*(?:(class)|(def)|(var)|([a-zA-Z_]+[a-zA-Z_0-9]*)|(->)|"
    "([0-9]+\\.[0-9]*|\\.[0-9]+)|([0-9]+)|'(.)'|\"(.*)\"|(true|false)|"
    "(<)|(<=)|(>)|(>=)|(==)|(!=)|"
    "(&&)|(\\|\\|)|(;)|(\\()|(\\))|"
    "(,)|(=)|(!)|(+)|(-)|(*)|(/)|(%)", std::regex::ECMAScript | std::regex::optimize);

enum TokenTypes {
    T_NONE = 0,
    T_CLASS, T_DEF, T_VAR, T_IDENTIFIER, T_ARROW,
    T_FLOAT_LIT, T_INTEGER_LIT, T_CHAR_LIT, T_STRING_LIT, T_BOOL_LIT,
    T_LT, T_LE, T_GT, T_GE, T_EQ, T_NE,
    T_AND, T_OR, T_SEMICOLON, T_LPAREN, T_RPAREN,
    T_COMMA, T_EQUAL, T_NOT, T_PLUS, T_MINUS, T_MUL, T_DIV, T_MODULO
};

} // namespace

namespace parser {

bool tokenize(std::vector<TokenInfo>& tokens, std::string& line, std::istream& s) {
    bool parsed_statement = false;

    while (s) {
        // Feed line
        if (line.empty()) {
            std::string new_line;
            std::getline(s, new_line);
            line.append(new_line);
            line.push_back('\n');
        }

        for(std::sregex_iterator it(std::begin(line), std::end(line), token_re), end_it; it != end_it; ++it) {
            auto& elem = *it;
            for(unsigned int i = 1; i < elem.max_size(); ++i) {
                if(elem[i].matched) {
                    tokens.push_back(std::make_pair((TokenTypes)i, elem[i].str()));
                    if (i == T_SEMICOLON)
                        parsed_statement = true;
                    break;
                }
            }
        }

        if (parsed_statement)
            return true;
    }

    return false;
}

BaseNode* parse(std::vector<TokenInfo>& tokens) {
    // TODO
    return nullptr;
}

} // namespace parser
