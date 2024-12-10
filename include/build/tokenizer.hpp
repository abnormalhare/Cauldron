#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "resultT.hpp"
#include "build/token.hpp"

enum TokenizerState {
    STATE_BAD = -1,

    STATE_FILE,
    STATE_CLASS,
    STATE_IMPL,
    STATE_FUNC,

};

class Tokenizer {
    private:
        std::ifstream& file;
        std::vector<TokenType> token;

        TokenizerState currState;

        TokenType _dttFile(std::string value);

    public:
        Tokenizer(std::ifstream& file);
        TokenType determineTokenType(std::string value);
        Token getToken(std::string value);
        void tokenize();
};