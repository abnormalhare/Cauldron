#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::ifstream& file) : file(file) {}

void Tokenizer::setCurrNodeAccess(TokenType tokenType) {
    this->allowedWords = {
        "class", "struct", "enum", "trait", "type"
    };

    if (this->nodeStruct->parent == nullptr) {
        this->nodeStruct->tokens.push_back(tokenType);
    } else {
        this->allowedWords.push_back("func");
        this->allowedWords.push_back("var");

        this->nodeStruct->tokens.push_back(tokenType);
    }
}

bool Tokenizer::isTokenTypeValid(std::string value) {
    if (!std::count(allowedWords.begin(), allowedWords.end(), value)) {
        return false;
    }

    std::cout << value << std::endl;

    if (allowedWords[0] == "__any") {
        if (this->nodeStruct->tokens[1] == FUNC) {
            if (std::find(funcOpNames.begin(), funcOpNames.end(), value) == funcOpNames.end()) {
                this->nodeStruct->tokens.push_back(OP_FUNC);
                this->nodeStruct->name = value;

                Node node = {this->nodeStruct, {}, {}};
                std::shared_ptr<Node> snode = std::make_shared<Node>(node);
                this->nodeStruct->children.push_back(snode);
            }
        }
    }

    if      (value == "public") this->setCurrNodeAccess(PUBLIC);
    else if (value == "private") this->setCurrNodeAccess(PRIVATE);
    else if (value == "protected") this->setCurrNodeAccess(PROTECTED);
    else if (value == "internal") this->setCurrNodeAccess(INTERNAL);
    else if (value == "class") {
        this->nodeStruct->tokens.push_back(CLASS);

        allowedWords = {"__any"};
    }

    return true;
}

void Tokenizer::getToken(std::string value) {
    std::string tokenValue;

    for (size_t index = 0; index < value.size(); index++) {
        std::cout << value[index] << std::endl;
        if (isspace(value[index])) continue;
        tokenValue.push_back(value[index]);

        if (this->isTokenTypeValid(tokenValue)) {
            tokenValue = "";
        }
    }
    if (tokenValue != "") {
        exit(CDRES_TOKEN);
    }
}

void Tokenizer::tokenize() {
    std::cout << "Tokenizing file..." << std::endl;
    while (!this->file.eof()) {
        std::string line;
        std::getline(this->file, line);
        this->getToken(line);
    }
}