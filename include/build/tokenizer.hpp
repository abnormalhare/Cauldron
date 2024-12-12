#pragma once

#include <fstream>
#include <ctype.h>
#include <cstdarg>
#include <iterator>

#include "base.hpp"
#include "resultT.hpp"
#include "debug.hpp"
#include "build/tokentype.hpp"
#include "build/token.hpp"

const std::array<std::string, 28> disallowedNames = {
    "struct", "union", "enum", "class", "trait", "impl",
    "func",
    "&", "*",
    "if", "else", "while", "for", "break", "continue", "ret", "switch", "case",
    "overload", "override", "const", "var", "type",
    "public", "private", "protected", "internal",
    "self",
};

const std::array<char, 27> disallowedSymbols {
    '+', '-', '*', '/', '%', '!', '&', '|', '^', '<',
    '>', '=', '?', ':', ';', ',', '(', ')', '{', '}',
    '[', ']', '.', ' ', '\t', '\n', '\r'
};


const std::array<std::string, 22> funcOpNames = {
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "&", "|", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
};

const std::array<std::string, 60> disallowedFuncOpNames = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "f128",
    "bool", "string", "func", "date", "array", "vararray", "hashmap",
    "struct", "union", "enum", "class", "trait", "impl",
    "func",
    "&",
    "if", "else", "while", "for", "break", "continue", "ret", "switch", "case",
    "overload", "override", "const", "var", "type",
    ";", ":", ".", ",",
    "(", ")", "{", "}", "[", "]", "//", "/*", "*/",
    "public", "private", "protected", "internal",
    "self",
};


#define areSymbolsInVal(value, symbols) \
    (std::any_of(value.begin(), value.end(), [](char c) { return isValInArray(c, symbols); }))

const std::vector<std::string> baseAllowedTokens = {
    "public", "private", "protected", "internal",
    "class", "struct", "enum", "trait",
    "func", "var", "const", "type",
    "}"
};

const std::vector<std::string> baseAllowedTypes = {
    "void", "s8", "u8", "char", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "f128",
    "bool", "string", "func", "date", "array", "vararray", "hashmap",
};

class Tokenizer {
    private:
        std::string fileName;
        std::ifstream& file;

        std::vector<Token*> baseTokens;
        Token* currToken;
        
        Token* compareAgainst;
        std::vector<std::string> allowedValues = baseAllowedTokens;

        std::string badName;
        TokenType badToken;

        void setTokenName(std::string value);

        void setTokenTypeAccess(TokenType type);
        void setTokenTypeOOP(TokenType type);

        void setTokenType(std::string value);
        void setToken(char& value);
        void setToken(std::string& value);
        void determineTokenLine(std::string line);

        void callDebugPrint();
        void handleError(Result res);

    public:
        Tokenizer(std::string fileName, std::ifstream& file);
        void tokenize();
};