#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::ifstream& file) : file(file) {}



TokenType Tokenizer::determineTokenType(std::string value) {
    if (value == "//") {
        return LINE_COMMENT;
    } else if (value == "/*") {
        return COMMENT_BEGIN;
    } else if (value == "*/") {
        return COMMENT_END;
    }

    switch (currState) {
        case STATE_FILE: return _dttFile(value);
    }
}

Token Tokenizer::getToken(std::string value) {
    bool isToken = false;
    std::string tokenValue;

    for (int index; index < value.size();) {
        while (!isToken) {
            tokenValue.push_back(value[index]);

            isToken = this->determineTokenType(tokenValue);
            index++;
        }
    }
    if (tokenValue != "") {
        exit(CDRES_TOKEN);
    }
}

void Tokenizer::tokenize() {
    while (!this->file.eof()) {
        std::string line;
        std::getline(this->file, line);
        getToken(line);
    }
}