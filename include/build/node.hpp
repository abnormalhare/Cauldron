#pragma once

#include <memory>
#include <vector>

#include "build/token.hpp"

class Node {
    public:
        Node* parent;
        std::vector<TokenType> tokens;
        std::string name;
        std::vector<Node*> children;

        Node(Node* parent, std::vector<TokenType> tokens, std::string name, std::vector<Node*> children);

        Node* newChild();
        void add(TokenType token);
};