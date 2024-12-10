#pragma once

#include <iostream>
#include <fstream>
#include "resultT.hpp"
#include "build/tokenizer.hpp"

class Parser {
    private:
        const char* fileName;
        std::ifstream file;
        Tokenizer tokenizer;

    public:
        Parser(const char* fileName);
        void parse();
};