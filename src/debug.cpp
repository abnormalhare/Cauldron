#include "debug.hpp"

const std::array<std::string, 82> printNames = {
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
    "[file]", "[type]", "[parameter]", "[function]", "[class]", "[trait]", "[impl]",
    "[line comment]", "[comment]",
    "public", "private", "protected", "internal",
    "self",
};

int numLines = 0;
void printNode(Token* token) {
    std::string indent;
    for (int i = 0; i < numLines; i++) {
        indent += "| ";
    }

    std::cout << indent << printNames[token->type] << ": \"" << token->value << "\"" << std::endl;
}

void debugPrint(Token* token) {
    // print all nodes
    printNode(token);
    numLines++;
    for (auto child : token->children) {
        debugPrint(child);
    }
    numLines--;
};