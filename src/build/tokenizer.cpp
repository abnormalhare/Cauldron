#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::string fileName, std::ifstream& file) : fileName(fileName), file(file) {
    // create the first token that should NEVER BE LEFT
    this->currToken = new Token(nullptr, FILE_DEF, fileName);
    this->baseTokens.push_back(this->currToken);
}

void Tokenizer::setTokenName(std::string value) {
    Token* token = this->currToken;
    // set the token's name
    token->value = value;

    // because we've set the token's name, the values that come next
    // should be different
    switch (this->compareAgainst->type) {
        case TRAIT:
            this->allowedValues = {
                "<", "{"
            };
            break;
        case GENERIC_DEF:
            this->allowedValues = {
                ">"
            };
            break;

        default:
            handleError(CDRES_UNIMPLEMENTED);
    }
}

void Tokenizer::setTokenTypeAccess(TokenType type) {
    // add the token
    this->currToken->add(type);
    if (this->baseTokens.size() != 0) {
        this->allowedValues = {
            "trait", "class", "impl", "func", "var"
        };
    } else {
        this->allowedValues = {
            "trait", "class", "impl"
        };
    }
}

void Tokenizer::setTokenTypeOOP(TokenType type) {
    Token* oop = this->currToken->add(type);

    if (this->currToken->children.size() < 2) {
        setChild(this->currToken, 0);
        this->currToken->add(PRIVATE);
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

    this->badName = value;

    if (isValInArray("__str", this->allowedValues)) {
        std::vector<std::string> prevAV;
        mycopy(this->allowedValues, prevAV);

        this->setTokenName(value);
        if (prevAV.size() == 1) return;
    }

    if (!isValInArray(value, this->allowedValues)) handleError(CDRES_SYNTAX);


    if      (value == "public")    this->setTokenTypeAccess(PUBLIC);
    else if (value == "private")   this->setTokenTypeAccess(PRIVATE);
    else if (value == "internal")  this->setTokenTypeAccess(INTERNAL);
    else if (value == "protected") this->setTokenTypeAccess(PROTECTED);

    else if (value == "trait")     this->setTokenTypeOOP(TRAIT);
    else if (value == "class")     this->setTokenTypeOOP(CLASS);
    else if (value == "impl")      this->setTokenTypeOOP(IMPL);

    else if (value == "func") {
        if (!isOOP(this->currToken->type)) {
            
        }
    }

    else if (value == "<") {
        if (this->currToken->parent->type == IF) {
            this->currToken->add(LT);
            return;
        }

        Token* token = this->currToken->add(GENERIC_DEF);
        setChildIB(this->currToken, 1);

        this->compareAgainst = token;
        this->allowedValues = { "__str" };
    } else if (value == ">") {
        if (this->currToken->parent->type == IF) {
            this->currToken->add(GT);
        }
        setParent(this->currToken);
        
        this->allowedValues = {
            "{"
        };
    } else if (value == "=") {
        Token* prevToken = this->currToken->children.back();
        if (prevToken->type == LT) {
            prevToken->type = LE;
        } else if (prevToken->type == GT) {
            prevToken->type = GE;
        } else if (prevToken->type == TYPE_IDENT) {
            this->currToken->add(DEFINE);
        }
    } else if (value == "{") {
        TokenType type = this->currToken->type;

        switch (type) {
            case TRAIT: addSetChild(this->currToken, TRAIT_DEF); break;
            case CLASS: addSetChild(this->currToken, CLASS_DEF); break;
            case IMPL:  addSetChild(this->currToken, IMPL_DEF); break;
            case FUNC:  addSetChild(this->currToken, FUNCTION_DEF); break;
            default: handleError(CDRES_SYNTAX);
        }

        this->baseTokens.push_back(this->currToken);
    } else {
        handleError(CDRES_SYNTAX);
    }
    
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
    if (res == CDRES_OK) return;

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
    debugPrint(this->baseTokens[0]);
}