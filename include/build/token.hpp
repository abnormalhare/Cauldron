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

        Token* add(TokenType type, std::string value);
        Token* add(Token* token);
        void remove(int pos);
        void remove(Token* token);
        // @param cutTokenPos the position of the token you want to cut
        // @param pasteTokenPos the position of the token you want to put it into
        void cutToChild(int cutTokenPos, int pasteTokenPos);
        void cutToChild(Token* cutToken, Token* pasteToken);
};

#define setChild(token, pos) token = token->children[pos]