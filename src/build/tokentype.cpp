#include "build/tokentype.hpp"

bool isSeparator(char value) {
    return value == ';' || value == ',' || value == ':' || value == '.' || value == ' '
        || value == '{' || value == '}'
        || value == '(' || value == ')'
        || value == '[' || value == ']'
        || value == '<' || value == '>';
}

bool isSeparatorType(TokenType type) {
    return type == END || type == ITEM_SEPARATOR || type == TYPE_IDENT || type == MODULE_ACCESS
        || type == GENERIC_DEF || type == PARAMETER_DEF
        || type == FUNCTION_DEF || type == CLASS_DEF || type == TRAIT_DEF || type == IMPL_DEF
        || type == COMMENT_DEF;
}

bool isOOP(TokenType type) {
    return type == CLASS_DEF || type == TRAIT_DEF || type == IMPL_DEF;
}

bool isAccess(TokenType type) {
    return type == PUBLIC || type == PRIVATE || type == PROTECTED || type == INTERNAL;
}

bool isFunc(TokenType type) {
    return type == FUNC || type == OP_FUNC;
}