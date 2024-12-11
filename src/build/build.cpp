#include "build/token.hpp"

bool isSeparator(char value) {
    return value == ';' || value == ',' || value == ':' || value == '.' || value == ' '
        || value == '{' || value == '}'
        || value == '(' || value == ')'
        || value == '[' || value == ']'
        || value == '<' || value == '>';
}

bool isSeparatorType(TokenType type) {
    return type == END || type == ITEM_SEPARATOR || type == TYPE_IDENT || type == MODULE_ACCESS
        || type == TYPE_DEF || type == LAMBDA || type == PARAMETER_DEF
        || type == FUNCTION_DEF || type == CLASS_DEF || type == TRAIT_DEF || type == IMPL_DEF
        || type == COMMENT_DEF;
}

bool isOOP(TokenType type) {
    return type == CLASS || type == TRAIT || type == IMPL;
}