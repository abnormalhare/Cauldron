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
#include "debug.hpp"
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
    "=>",
    "(", ")", "{", "}", "[", "]", "//", "/*", "*/",
    "public", "private", "protected", "internal",
    "self",
};

#define isValInArray(value, array) \
    std::find(array.begin(), array.end(), value) != array.end()
#define areSymbolsInVal(value, symbols) \
    std::any_of(value.begin(), value.end(), [](char c) { return isValInArray(c, symbols); })
#define indexBackward(index) this->nodeStruct->tokens.size() index

#define currAccess this->nodeStruct->tokens[0]
#define currType this->nodeStruct->tokens[0]
#define currStructure this->nodeStruct->tokens.back()
#define currName this->nodeStruct->name
#define currParentAccess this->nodeStruct->parent->tokens[0]
#define currParentType this->nodeStruct->parent->tokens[0]
#define currParentStructure this->nodeStruct->parent->tokens[1]
#define currParentName this->nodeStruct->parent->name

const std::vector<std::string> baseAllowedTokens = {
    "public", "private", "protected", "internal",
    "class", "struct", "enum", "trait",
    "func", "var", "const", "type",
    "}"
};

class Tokenizer {
    private:
        std::ifstream& file;
        std::vector<std::string> allowedWords = baseAllowedTokens;
        std::shared_ptr<Node> root;
        std::shared_ptr<Node> nodeStruct;
        bool isNameReady = false;
        bool isNameComplete = false;
        
        std::string badName;
        TokenType badToken;

        std::shared_ptr<Node> createChildNode();

        void nsAdd(TokenType token);
        void nsSetParent();

        bool isValueName(std::string value, Result& res);
        void setCurrNodeAccess(TokenType tokenType);
        void setCurrNodeStructure(TokenType tokenType);
        void setCurrNodeFunction(TokenType tokenType);
        void setCurrNodeCompare(TokenType tokenType);

        void callDebugPrint();

    public:
        Tokenizer(std::ifstream& file);
        bool isTokenTypeValid(std::string value, Result& res);
        void getToken(std::string value);
        void handleError(Result res);
        void tokenize();
};