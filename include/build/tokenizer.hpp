#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <memory>
#include <array>
#include <string>

#include "resultT.hpp"
#include "build/token.hpp"

const std::array<std::string, 85> disallowedNames = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "f128",
    "bool", "string", "func", "date", "array", "vararray", "hashmap",
    "struct", "union", "enum", "class", "trait", "impl",
    "func",
    "&", "*",
    "if", "else", "while", "for", "break", "continue", "ret", "switch", "case",
    "overload", "override", "const", "var", "type",
    ";", ":", ".", ",",
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "&", "|", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
    "=>",
    "<", ">", "(", ")", "{", "}", "[", "]", "//", "/*", "*/",
    "public", "private", "protected", "internal",
    "self",
};

const std::array<std::string, 82> printNames = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "f128",
    "bool", "string", "func", "date", "array", "vararray", "hashmap",
    "struct", "union", "enum", "class", "trait", "impl",
    "func",
    "&", "*",
    "if", "else", "while", "for", "break", "continue", "ret", "switch", "case",
    "overload", "override", "const", "var", "type",
    ";", ":", ".", ",",
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "&", "|", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
    "=>",
    "[type]", "[parameter]", "[function]", "[class]", "[trait]", "[impl]",
    "[line comment]", "[comment]",
    "public", "private", "protected", "internal",
    "self",
};

const std::array<std::string, 22> funcOpNames = {
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "&", "|", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
};

struct Node {
    std::shared_ptr<Node> parent;
    std::vector<TokenType> tokens;
    std::string name;
    std::vector<std::shared_ptr<Node>> children;
};

class Tokenizer {
    private:
        std::ifstream& file;
        std::vector<std::string> allowedWords = {
            "public", "private", "protected", "internal",
            "class", "struct", "enum", "trait",
            "func", "var", "const", "type",
        };
        std::shared_ptr<Node> nodeStruct;
        bool isNameReady = false;
        bool isNameComplete = false;

        void setCurrNodeAccess(TokenType tokenType);
    public:
        Tokenizer(std::ifstream& file);
        bool isTokenTypeValid(std::string value, Result& res);
        void getToken(std::string value, Result& res);
        void handleError(Result res);
        void tokenize();
};