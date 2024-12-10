#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <memory>

#include "resultT.hpp"
#include "build/token.hpp"

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
       std::shared_ptr<Node> nodeStruct = std::make_shared<Node>(nullptr, {}, {});

        void setCurrNodeAccess(TokenType tokenType);
    public:
        Tokenizer(std::ifstream& file);
        bool isTokenTypeValid(std::string value);
        void getToken(std::string value);
        void tokenize();
};