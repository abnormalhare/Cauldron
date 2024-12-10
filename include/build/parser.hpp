#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "resultT.hpp"

class Parser {
    private:
        const char* fileName;
        FILE* file;
    public:
        Parser(const char* fileName);
        void parse();
};

#endif