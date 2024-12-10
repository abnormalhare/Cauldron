#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "resultT.hpp"
#include "build/token.hpp"

class Tokenizer {
    private:
        std::ifstream& file;
        std::vector<TokenType> token;
    public:
        Tokenizer(std::ifstream& file);
        void tokenize();
};