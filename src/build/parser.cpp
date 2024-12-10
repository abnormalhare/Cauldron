#include "build/parser.hpp"

Parser::Parser(const char* fileName) : fileName(fileName), file(fileName), tokenizer(file) {

    if (!this->file.is_open()) {
        printf("Error: Could not open file %s\n", fileName);
        exit(CDRES_FILE);
    }
};

void Parser::parse() {
    this->tokenizer.tokenize();
};