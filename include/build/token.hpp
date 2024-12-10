#pragma once

#include <string>

enum TokenType {
    BAD_TOKEN = -1,

    // types
    VOID, S8, U8_CHAR, S16, U16, S32, U32, S64, U64, F32, F64, F128,
    BOOL, STRING, FUNC, DATE, ARRAY, VARARRAY, HASHMAP,
    STRUCT, UNION, ENUM, CLASS, TRAIT, IMPL,

    // pointers
    REF, PTR,

    // keywords
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, RET, SWITCH, CASE,
    OVERLOAD, OVERRIDE, CONST,

    // operators
    ADD, SUB, MUL, DIV, MOD, INC, DEC, NEG, NOT, AND, OR, XOR, SHL, SHR,
    EQ, NE, LT, GT, LE, GE,

    // protectors
    PUBLIC, PRIVATE, PROTECTED, INTERNAL,
};

typedef struct Token {
    TokenType type;
    std::string value;
};