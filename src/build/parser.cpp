#include "build/parser.hpp"

Parser::Parser(std::string fileName) : fileName(fileName), file(fileName), tokenizer(fileName, file) {
    if (!this->file.is_open()) {
        printf("Error: Could not open file %s\n", fileName.c_str());
        exit(CDRES_FILE);
    }
};

void Parser::parse() {
    this->tokenizer.tokenize();
};