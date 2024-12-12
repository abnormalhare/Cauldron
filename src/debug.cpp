#include "debug.hpp"

const std::array<std::string, 81> printNames = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "f128",
    "bool", "string", "func", "date", "array", "vararray", "hashmap",
    "struct", "union", "enum", "class", "trait", "impl",
    "op func",
    "&", "*",
    "if", "else", "while", "for", "break", "continue", "ret", "switch", "case",
    "overload", "override", "const", "var", "type",
    ";", ":", ".", ",",
    "+", "-", "*", "/", "%", "++", "--", "-", "!", "&&", "||", "&", "|", "^", "<<", ">>",
    "==", "!=", "<", ">", "<=", ">=",
    "[type]", "[parameter]", "[function]", "[class]", "[trait]", "[impl]",
    "[line comment]", "[comment]",
    "public", "private", "protected", "internal",
    "self",
};

int numLines = 0;
void printNode(Node* node) {
    std::string indent;
    for (int i = 0; i < numLines; i++) {
        indent += "| ";
    }

    std::cout << indent << "\"" << node->name << "\"" << std::endl;
    for (auto& token : node->tokens) {
        std::cout << indent << "| " << printNames[token] << std::endl;
    }
}

void debugPrint(Node* node) {
    // print all nodes
    printNode(node);
    numLines++;
    for (auto& child : node->children) {
        debugPrint(child);
    }
    numLines--;
};