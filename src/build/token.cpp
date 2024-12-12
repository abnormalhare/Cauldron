#include "build/token.hpp"

Token::Token(Token* parent, TokenType type, std::string value) {
    this->parent = parent;
    this->children = {};
    this->type = type;
    this->value = value;
}


Token* Token::add(TokenType type, std::string value) {
    Token* token = new Token(this, type, value);
    this->children.push_back(token);
    return token;
}

Token* Token::add(Token* token) {
    this->children.push_back(token);
    return token;
}

void Token::remove(int pos) {
    this->children.erase(this->children.begin() + pos);
}

void Token::remove(Token* token) {
    auto findToken = find(token, this->children);
    
    if (findToken != this->children.end()) {
        this->children.erase(findToken);
    }
}

void Token::cutToChild(int cutTokenPos, int pasteTokenPos) {
    Token* cut = this->children[cutTokenPos];
    this->children[pasteTokenPos]->children.push_back(cut);
    this->remove(cut);
}

void Token::cutToChild(Token* cutToken, Token* pasteToken) {
    pasteToken->children.push_back(cutToken);
    this->remove(cutToken);
}