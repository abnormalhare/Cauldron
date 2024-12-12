#include "build/node.hpp"

Node::Node(Node* parent, std::vector<TokenType> tokens, std::string name, std::vector<Node*> children) {
    this->parent = parent;
    this->tokens = tokens;
    this->name = name;
    this->children = children;
}

Node* Node::newChild() {
    // create a node with current node as its parent
    Node node = {this, {}, "", {}};
    Node* snode = new Node(node);
    // add the node to the children of the current node
    this->children.push_back(snode);
    return snode;
}

void Node::add(TokenType token) {
    this->tokens.push_back(token);
}