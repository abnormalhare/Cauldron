#include "parser.hpp"

Parser::Parser(const char* fileName) {
    this->fileName = fileName;
    this->file = fopen(fileName, "r");

    if (this->file == NULL) {
        printf("Error: Could not open file %s\n", fileName);
        exit(CDRES_FILE);
    }
};

