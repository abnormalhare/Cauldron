#pragma once

#include <memory>
#include <vector>

#include "base.hpp"
#include "build/tokentype.hpp"

class Token {
    public:
        Token* parent;
        std::vector<Token*> children;
        TokenType type;
        std::string value;

        Token(Token* parent, TokenType type, std::string value);

        Token* add(TokenType type, std::string value = "");
        Token* add(Token* token);
        Token* get(Token* token, int offset = 0);
        void remove(int pos);
        void remove(Token* token);
        void replace(Token* from, Token* to);
        // @param cutTokenPos the position of the token you want to cut
        // @param pasteTokenPos the position of the token you want to put it into
        void cutToChild(int cutTokenPos, int pasteTokenPos);
        void cutToChild(Token* cutToken, Token* pasteToken);

};

#define setChild(from, to) \
    auto __tokenI = find(to, from->children); \
    if (__tokenI != from->children.end()) { \
        int i = std::distance(from->children.begin(), __tokenI);\
        from = from->children[i]; \
    } \

#define setParent(token) token = token->parent
#define addSetChild(token, type) Token* __cToken = token->add(type); setChild(token, __cToken)