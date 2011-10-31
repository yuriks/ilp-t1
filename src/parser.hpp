#pragma once

#include <string>
#include <vector>

namespace parser {

enum NodeType {
	NODE_TYPE_DEF,
	NODE_FUNC_DEF,
	NODE_VAR_DEF
};

struct BaseNode {
	NodeType type;
};

struct TypeDefNode : BaseNode {
	std::string type_name;
};

struct FuncDefNode : BaseNode {
	std::string func_name;
	std::string return_type;
	std::vector<std::string> param_types;
};

enum ExpressionType {
	E_BINARY_OP, E_UNARY_OP, E_FUNCTION, E_VARIABLE, E_LITERAL
};

enum LiteralType {
	LIT_FLOAT, LIT_INT, LIT_CHAR, LIT_STRING, LIT_BOOL
};

struct ExpressionNode {
	ExpressionType type;

	// if E_BINARY_OP, E_UNARY_OP or E_FUNCTION
	std::vector<ExpressionNode> parameters;
	// if E_FUNCTION or E_VARIABLE
	std::string name;
	// if E_LITERAL
	LiteralType literal_type;
};

struct VarDefNode : BaseNode {
	std::string var_name;
	ExpressionNode value;
};

} // namespace parser