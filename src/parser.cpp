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

FunctionDeclaration = 'def' Identifier '(' ( ')' | ParameterListDecl ) '->' Type
ParameterListDecl = Type ( ')' | ',' ParameterListDecl )

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
#include <stdexcept>

namespace parser {

static const std::regex token_re(
    "\\s*(?:(class)|(def)|(var)|([a-zA-Z_]+[a-zA-Z_0-9]*)|(->)|"
    "(\\d+\\.\\d*|\\.\\d+)|(\\d+)|'(.)'|\"(.*)\"|(true|false)|"
    "(<)|(<=)|(>)|(>=)|(==)|(!=)|"
    "(&&)|(\\|\\|)|(;)|(\\()|(\\))|"
    "(,)|(=)|(!)|(\\+)|(-)|(\\*)|(\\/)|(%))", std::regex::ECMAScript | std::regex::optimize);

enum TokenTypes {
    T_NONE = 0,
    T_CLASS, T_DEF, T_VAR, T_IDENTIFIER, T_ARROW,
    T_FLOAT_LIT, T_INTEGER_LIT, T_CHAR_LIT, T_STRING_LIT, T_BOOL_LIT,
    T_LT, T_LE, T_GT, T_GE, T_EQ, T_NE,
    T_AND, T_OR, T_SEMICOLON, T_LPAREN, T_RPAREN,
    T_COMMA, T_EQUAL, T_NOT, T_PLUS, T_MINUS, T_MUL, T_DIV, T_MODULO
};

bool tokenize(std::vector<TokenInfo>& tokens, std::string& line) {
    bool parsed_statement = false;

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

    return parsed_statement;
}

std::shared_ptr<TypeDefNode> parseTypeDeclaration(std::vector<TokenInfo>& tokens, unsigned int& cur_token_index) {
    if(tokens[cur_token_index].first == T_IDENTIFIER) {
        auto n = std::make_shared<TypeDefNode>();
        n->type = NODE_TYPE_DEF;
        n->type_name = tokens[cur_token_index].second;
        ++cur_token_index;
        return n;
    } else {
        throw std::runtime_error("Error: Expected identifier.");
    }
}

std::shared_ptr<FuncDefNode> parseFunctionDeclaration(std::vector<TokenInfo>& tokens, unsigned int& cur_token_index) {
    if (tokens[cur_token_index].first != T_IDENTIFIER) {
        throw std::runtime_error("Error: expected identifier.");
    }

    auto fdef = std::make_shared<FuncDefNode>();
    fdef->type = NODE_FUNC_DEF;
    fdef->func_name = tokens[cur_token_index].second;
    ++cur_token_index;

    if (tokens[cur_token_index].first != T_LPAREN) {
        throw std::runtime_error("Error: expected '('.");
    }
    ++cur_token_index;

    if (tokens[cur_token_index].first != T_RPAREN) {
        do {
            if (tokens[cur_token_index].first != T_IDENTIFIER) {
                throw std::runtime_error("Error: expected type.");
            }
            fdef->param_types.push_back(tokens[cur_token_index].second);
            ++cur_token_index;
        } while (tokens[cur_token_index++].first == T_COMMA);
    }

    if (tokens[cur_token_index].first != T_RPAREN) {
        throw std::runtime_error("Error: expected ')' or ','.");
    }
    ++cur_token_index;

    if (tokens[cur_token_index].first != T_ARROW) {
        throw std::runtime_error("Error: expected Type, found nothing.");
    }
    ++cur_token_index;

    if (tokens[cur_token_index].first != T_IDENTIFIER) {
        throw std::runtime_error("Error: expected Type, found invalid symbol.");
    }
    fdef->return_type = tokens[cur_token_index].second;
    ++cur_token_index;

    return fdef;
}

ExpressionNode parseExpression(std::vector<TokenInfo>& tokens, unsigned int& i);

bool isLiteral(TokenTypes token) {
    return token == T_FLOAT_LIT || token == T_INTEGER_LIT ||
        token == T_CHAR_LIT  || token == T_STRING_LIT  ||
        token == T_BOOL_LIT;
}

LiteralType literalType(TokenTypes token) {
   LiteralType t;

   switch(token) {
        case T_FLOAT_LIT:
            t = LIT_FLOAT;
            break;
        case T_INTEGER_LIT:
            t = LIT_INT;
            break;
        case T_CHAR_LIT:
            t = LIT_CHAR;
            break;
        case T_STRING_LIT:
            t = LIT_STRING;
            break;
        case T_BOOL_LIT:
            t = LIT_BOOL;
            break;
        default:
            break; //TODO handle error
    }

    return t;
}

bool isFunctionCall(std::vector<TokenInfo>& tokens, unsigned int& i) {
    return tokens[i].first == T_LPAREN;
}

std::vector<ExpressionNode> parseFunctionCallParameters(std::vector<TokenInfo>& tokens, unsigned int& i) {
    std::vector<ExpressionNode> ret;
    if (tokens[i].first != T_RPAREN) {
        while (true) {
            ret.push_back(parseExpression(tokens, i));
            if (tokens[i].first == T_RPAREN) {
                ++i;
                break;
            } else if (tokens[i].first != T_COMMA) {
                throw std::runtime_error("Error: expected , or ).");
            }
            ++i;
        }
    }
    return ret;
}

ExpressionNode parseTier0(std::vector<TokenInfo>& tokens, unsigned int& i) {
    TokenTypes ntoken = tokens[i].first;
    if (isLiteral(ntoken)) {
        ++i;
        ExpressionNode n;
        n.type = E_LITERAL;
        n.literal_type = literalType(ntoken);

       return n;
    } else if (ntoken == T_IDENTIFIER) {
        ExpressionNode n;
        n.name = tokens[i].second;
        ntoken = tokens[++i].first;
        
        if(isFunctionCall(tokens, i)) {
            n.type = E_FUNCTION;
            ++i;
            n.parameters = parseFunctionCallParameters(tokens, i);
            ++i;
            return n;
        } else {
            n.type = E_VARIABLE;
            ++i;
            return n;
        }
        
    } else {
        return parseExpression(tokens, i);
    }
}

ExpressionNode parseTier1(std::vector<TokenInfo>& tokens, unsigned int& i) {
    TokenTypes ntoken = tokens[i].first;
    if (ntoken == T_NOT || ntoken == T_MINUS) {
        ++i;

        ExpressionNode n;
        n.type = E_UNARY_OP;
        n.parameters.push_back(parseTier0(tokens, i));

        if (ntoken == T_NOT)
            n.name = "!";
        else if (ntoken == T_MINUS)
            n.name = "-";
        return n;
    } else {
        return parseTier0(tokens, i);
    }
}

ExpressionNode parseTier2(std::vector<TokenInfo>& tokens, unsigned int& i) {
    ExpressionNode lhs = parseTier1(tokens, i);

    TokenTypes ntoken = tokens[i].first;
    if (ntoken == T_MUL || ntoken == T_DIV || ntoken == T_MODULO) {
        ++i;

        ExpressionNode rhs = parseTier1(tokens, i);

        ExpressionNode n;
        n.type = E_BINARY_OP;
        n.parameters.push_back(lhs);
        n.parameters.push_back(rhs);

        if (ntoken == T_MUL)
            n.name = "*";
        else if (ntoken == T_DIV)
            n.name = "/";
        else if (ntoken == T_MODULO)
            n.name = "%";
        return n;
    } else {
        return lhs;
    }
}

ExpressionNode parseTier3(std::vector<TokenInfo>& tokens, unsigned int& i) {
    ExpressionNode lhs = parseTier2(tokens, i);

    TokenTypes ntoken = tokens[i].first;
    if (ntoken == T_PLUS || ntoken == T_MINUS) {
        ++i;

        ExpressionNode rhs = parseTier2(tokens, i);

        ExpressionNode n;
        n.type = E_BINARY_OP;
        n.parameters.push_back(lhs);
        n.parameters.push_back(rhs);

        if (ntoken == T_PLUS)
            n.name = "+";
        else if (ntoken == T_MINUS)
            n.name = "-";
        return n;
    } else {
        return lhs;
    }
}

ExpressionNode parseTier4(std::vector<TokenInfo>& tokens, unsigned int& i) {
    ExpressionNode lhs = parseTier3(tokens, i);

    TokenTypes ntoken = tokens[i].first;
    if (ntoken == T_LT || ntoken == T_LE || ntoken == T_GT || ntoken == T_GE) {
        ++i;

        ExpressionNode rhs = parseTier3(tokens, i);

        ExpressionNode n;
        n.type = E_BINARY_OP;
        n.parameters.push_back(lhs);
        n.parameters.push_back(rhs);

        if (ntoken == T_LT)
            n.name = "<";
        else if (ntoken == T_LE)
            n.name = "<=";
        else if (ntoken == T_GT)
            n.name = ">";
        else if (ntoken == T_GE)
            n.name = ">=";
        return n;
    } else {
        return lhs;
    }
}

ExpressionNode parseTier5(std::vector<TokenInfo>& tokens, unsigned int& i) {
    ExpressionNode lhs = parseTier4(tokens, i);

    TokenTypes ntoken = tokens[i].first;
    if (ntoken == T_EQ || ntoken == T_NE) {
        ++i;

        ExpressionNode rhs = parseTier4(tokens, i);

        ExpressionNode n;
        n.type = E_BINARY_OP;
        n.parameters.push_back(lhs);
        n.parameters.push_back(rhs);
        if (ntoken == T_EQ)
            n.name = "==";
        else if (ntoken == T_NE)
            n.name = "!=";
        return n;
    } else {
        return lhs;
    }
}

ExpressionNode parseTier6(std::vector<TokenInfo>& tokens, unsigned int& i) {
    ExpressionNode lhs = parseTier5(tokens, i);

    if (tokens[i].first == T_AND) {
        ++i;

        ExpressionNode rhs = parseTier5(tokens, i);

        ExpressionNode n;
        n.type = E_BINARY_OP;
        n.parameters.push_back(lhs);
        n.parameters.push_back(rhs);
        n.name = "&&";
        return n;
    } else {
        return lhs;
    }
}

ExpressionNode parseTier7(std::vector<TokenInfo>& tokens, unsigned int& i) {
    ExpressionNode lhs = parseTier6(tokens, i);

    if (tokens[i].first == T_OR) {
        ++i;

        ExpressionNode rhs = parseTier6(tokens, i);

        ExpressionNode n;
        n.type = E_BINARY_OP;
        n.parameters.push_back(lhs);
        n.parameters.push_back(rhs);
        n.name = "||";
        return n;
    } else {
        return lhs;
    }
}

ExpressionNode parseExpression(std::vector<TokenInfo>& tokens, unsigned int& i) {
    bool read_paren = false;
    if (tokens[i].first == T_LPAREN) {
        ++i;
        read_paren = true;
    }

    ExpressionNode n = parseTier7(tokens, i);

    if (read_paren) {
        if (tokens[i].first != T_LPAREN) {
            throw std::runtime_error("Error: expected ')'");
        }
        ++i;
    }

    return n;
}

std::shared_ptr<VarDefNode> parseVarDeclaration(std::vector<TokenInfo>& tokens, unsigned int& cur_token_index) {
    auto n = std::make_shared<VarDefNode>();
    n->type = NODE_VAR_DEF;

    if (tokens[cur_token_index].first != T_IDENTIFIER) {
        throw std::runtime_error("Error: expected identifier.");
    }
    n->var_name = tokens[cur_token_index].second;
    ++cur_token_index;

    if (tokens[cur_token_index].first != T_EQUAL) {
        throw std::runtime_error("Error: expected '='.");
    }
    ++cur_token_index;

    n->value = parseExpression(tokens, cur_token_index);

    return n;
}

std::shared_ptr<BaseNode> parseDeclaration(std::vector<TokenInfo>& tokens, unsigned int& cur_token_index) {
    std::shared_ptr<BaseNode> n;
    if (tokens[cur_token_index].first == T_CLASS) {
        ++cur_token_index;
        n = parseTypeDeclaration(tokens, cur_token_index);
    } else if (tokens[cur_token_index].first == T_DEF) {
        ++cur_token_index;
        n = parseFunctionDeclaration(tokens, cur_token_index);
    } else if (tokens[cur_token_index].first == T_VAR) {
        ++cur_token_index;
        n = parseVarDeclaration(tokens, cur_token_index);
    } else {
        throw std::runtime_error("Error: Expected type declaration, function declaration " 
            "or variable declaration.");
    }

    return n;
}

std::shared_ptr<BaseNode> parseStatement(std::vector<TokenInfo>& tokens, unsigned int& cur_token_index) {
    std::shared_ptr<BaseNode> node = parseDeclaration(tokens, cur_token_index);
    if (tokens[cur_token_index].first != T_SEMICOLON) {
        throw std::runtime_error("Error: Expected a ;");
    }
    ++cur_token_index;
    return node;
}

std::shared_ptr<BaseNode> parse(std::vector<TokenInfo>& tokens) {
    unsigned int i = 0;
    std::shared_ptr<BaseNode> node = parseStatement(tokens, i);
    tokens.erase(tokens.begin(), tokens.begin()+i);
    return node;
}

} // namespace parser
