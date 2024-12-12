#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::string fileName, std::ifstream& file) : fileName(fileName), file(file) {
    // create rootToken node THAT SHOULD NEVER HAVE TOKENS OR A NAME
    this->currToken = this->rootToken = new Token(nullptr, FILE_DEF, fileName);
}

void Tokenizer::setTokenName(std::string value) {
    Token* token = this->currToken;
    token->value = value;

    switch (this->compareAgainst->type) {
        case TRAIT:
            this->allowedValues = {
                "<", "{"
            };
            break;
            
        default:
            handleError(CDRES_UNIMPLEMENTED);
    }
}

void Tokenizer::setTokenTypeAccess(TokenType type) {
    this->currToken->add(type, "");
    if (this->baseTokens.size() != 0) {
        this->allowedValues = {
            "trait", "class", "impl", "func", "var"
        };
    } else {
        this->allowedValues = {
            "trait", "class"
        };
    }
}

void Tokenizer::setTokenTypeOOP(TokenType type) {
    Token* oop = this->currToken->add(type, "");

    if (this->currToken->children.size() < 2) {
        setChild(this->currToken, 0);
        this->currToken->add(PRIVATE, "");
    } else {
        Token* access = this->currToken->children.front();

        this->currToken->cutToChild(access, oop);
        setChild(this->currToken, 0);
    }

    this->compareAgainst = oop;
    this->allowedValues = { "__str" };
}

void Tokenizer::setTokenType(std::string value) {
    if (value == "") return;

    if (isValInArray("__str", this->allowedValues)) {
        this->setTokenName(value);
        if (this->allowedValues.size() == 1) return;
    }

    if (!isValInArray(value, this->allowedValues)) return;


    if      (value == "public")    this->setTokenTypeAccess(PUBLIC);
    else if (value == "private")   this->setTokenTypeAccess(PRIVATE);
    else if (value == "internal")  this->setTokenTypeAccess(INTERNAL);
    else if (value == "protected") this->setTokenTypeAccess(PROTECTED);

    else if (value == "trait")     this->setTokenTypeOOP(TRAIT);
    else if (value == "class")     this->setTokenTypeOOP(CLASS);
    else if (value == "impl")      this->setTokenTypeOOP(IMPL);
    
}

void Tokenizer::setToken(char& value) {
    std::string v = {value};
    this->setTokenType(v);
    this->callDebugPrint();
    value = '\0';
}

void Tokenizer::setToken(std::string& value) {
    this->setTokenType(value);
    this->callDebugPrint();
    value = "";
}

void Tokenizer::determineTokenLine(std::string line) {
    std::string value;
    char currChar;

    for (size_t index = 0; index < line.size(); index++) {
        currChar = line[index];

        uint8_t sep = ((isspace(currChar) != 0) << 1) | isSeparator(currChar);
        if (sep == 0b00) { // is a standard character
            value.push_back(currChar);
        } else if (sep == 0b01) { // is a separator
            this->setToken(value);
            this->setToken(currChar);
        } else if (sep == 0b10 || sep == 0b11) { // is a space
            this->setToken(value);
        }
    }

    if (value.size() > 0) {
        this->badName = value;
        handleError(CDRES_SYNTAX);
    }
}

void Tokenizer::handleError(Result res) {
    if (res != CDRES_OK) return;

    switch (res) {
        case CDRES_ARGS:
            std::cout << "Error: Invalid arguments" << std::endl;
            break;
        case CDRES_FILE:
            std::cout << "Error: Invalid file" << std::endl;
            break;
        case CDRES_SYNTAX:
            std::cout << "Error: Invalid syntax: \"" << this->badName << "\" (" << this->badName.size() << ")" << std::endl;
            break;
        case CDRES_UNIMPLEMENTED:
            std::cout << "Error: Unimplemented feature: " << this->badToken << std::endl;
            break;
        default:
            break;
    }
    
    exit(res);
}

void Tokenizer::tokenize() {
    std::cout << "Tokenizing file..." << std::endl;
    while (!this->file.eof()) {
        std::string line;
        std::getline(this->file, line);
        this->determineTokenLine(line);
    }
}

void Tokenizer::callDebugPrint() {
    std::cout << "================" << std::endl;
    debugPrint(this->rootToken);
}