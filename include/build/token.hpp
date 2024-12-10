#pragma once

#include <string>
#include <array>

enum TokenType {
    BAD_TOKEN = -1,

    // types
    VOID, S8, U8_CHAR, S16, U16, S32, U32, S64, U64, F32, F64, F128,
    BOOL, STRING, FUNC, DATE, ARRAY, VARARRAY, HASHMAP,
    STRUCT, UNION, ENUM, CLASS, TRAIT, IMPL,

    // operator function
    OP_FUNC,

    // pointers
    REF, PTR,

    // keywords
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, RET, SWITCH, CASE,
    OVERLOAD, OVERRIDE, CONST, VAR, TYPE,

    END,

    // operators
    ADD, SUB, MUL, DIV, MOD, INC, DEC, NEG, NOT, AND, OR, XOR, SHL, SHR,
    EQ, NE, LT, GT, LE, GE,
    TYPE_DEF, FUNC_DEF,

    // parentheses
    TYPE_BEGIN, TYPE_END,
    PARAMETER_BEGIN, PARAMETER_END,
    FUNCTION_BEGIN, FUNCTION_END,
    LINE_COMMENT, COMMENT_BEGIN, COMMENT_END,


    // class words
    PUBLIC, PRIVATE, PROTECTED, INTERNAL,
    SELF,
};

std::array<std::string, 77> disallowedNames = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "f128",
    "bool", "string", "func", "date", "array", "vararray", "hashmap",
    "struct", "union", "enum", "class", "trait", "impl",
    "&", "*",
    "if", "else", "while", "for", "break", "continue", "ret", "switch", "case",
    "overload", "override", "const", "var", "type",
    ";",
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
    "type", "=>",
    "{", "}", "(", ")", "[", "]", "//", "/*", "*/",
    "public", "private", "protected", "internal",
    "self",
};

std::array<std::string, 20> funcOpNames = {
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
};