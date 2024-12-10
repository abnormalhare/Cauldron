#pragma once

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