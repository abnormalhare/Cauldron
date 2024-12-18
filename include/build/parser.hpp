#pragma once

#include <iostream>
#include <fstream>
#include "result.hpp"
#include "build/tokenizer.hpp"

class Parser {
    private:
        std::string fileName;
        std::ifstream file;
        Tokenizer tokenizer;

    public:
        Parser(std::string fileName);
        void parse();
};