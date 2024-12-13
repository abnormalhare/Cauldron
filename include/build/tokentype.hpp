#pragma once

#include <memory>
#include <vector>
#include <string>

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
    OVERLOAD, CONST, VAR, TYPE, STATIC,

    // symbols
    END, TYPE_IDENT, MODULE_ACCESS, ITEM_SEPARATOR, DEFINE, OOP_START,

    // operators
    ADD, SUB, MUL, DIV, MOD, PWR, INC, DEC, NEG, NOT, BAND, BOR, AND, OR, XOR, SHL, SHR,
    EQ, NE, LT, GT, LE, GE,

    // brackets
    FILE_DEF,
    GENERIC_DEF,
    PARAMETER_DEF,
    FUNCTION_DEF,
    BEFORE_DEF,
    CLASS_DEF,
    TRAIT_DEF,
    IMPL_DEF,
    LINE_COMMENT, COMMENT_DEF,

    // class words
    PUBLIC, PRIVATE, PROTECTED, INTERNAL,
    SELF,
};

bool isSeparator(char value);
bool isSeparatorType(TokenType type);
bool isOOP(TokenType type);
bool isAccess(TokenType type);
bool isFunc(TokenType type);