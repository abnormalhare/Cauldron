#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::string fileName, std::ifstream& file) : fileName(fileName), file(file) {
    // create the first token that should NEVER BE LEFT
    this->currToken = new Token(nullptr, FILE_DEF, fileName);
    this->baseTokens.push_back(this->currToken);
}

void Tokenizer::setTokenName(std::string value) {
    Token* token = this->currToken;

    // check for if the current name is allowed
    if (token->type != FUNC) {
        if (isValInArray(value, disallowedNames)) handleError(CDRES_SYNTAX_NOT_ALLOWED, "setTokenName+isValInArray");
        if (areSymbolsInVal(value, disallowedSymbols)) handleError(CDRES_SYNTAX_NOT_ALLOWED, "setTokenName+areSymbolsInVal");
    } else {
        if (isValInArray(value, disallowedFuncNames)) handleError(CDRES_SYNTAX_NOT_ALLOWED, "setTokenName+funcIsValInArray");
        if (areSymbolsInVal(value, disallowedFuncSymbols)) handleError(CDRES_SYNTAX_NOT_ALLOWED, "setTokenName+funcIsValInArray");
    }

    this->couldBeOpFunc = false;

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
        case PARAMETER_DEF:
            this->allowedValues = {
                ":"
            };
            break;
        case FUNC:
            this->allowedValues = {
                "("
            };
            break;

        default:
            handleError(CDRES_UNIMPLEMENTED, "setTokenName+switch");
    }
}

// sets an access token to be moved inside the next token
void Tokenizer::setTokenTypeAccess(TokenType type) {
    // add the token
    this->currToken->add(type);
    // if we are not in the base file
    if (this->baseTokens.size() > 1) {
        // allow functions and variables to have custom access
        this->allowedValues = {
            "trait", "class", "impl", "func", "var"
        };
    // otherwise,
    } else {
        // only allow OOP to have custom access
        this->allowedValues = {
            "trait", "class", "impl"
        };
    }
}

void Tokenizer::setTokenTypeOOP(TokenType type) {
    Token* oop = this->currToken->add(type);

    if (this->currToken->children.size() < 2) {
        setChild(this->currToken, oop);
        this->currToken->add(PRIVATE);
    } else {
        Token* access = this->currToken->children.front();

        this->currToken->cutToChild(access, oop);
        setChild(this->currToken, oop);
    }

    this->compareAgainst = oop;
    this->allowedValues = { "__str" };
}

void Tokenizer::setTokenType(std::string value) {
    if (value == "") return;

    std::vector<std::string> prevAV;
    mycopy(this->allowedValues, prevAV);

    this->badName = value;

    if (isValInArray("__str", this->allowedValues)) {
        this->setTokenName(value);
        if (prevAV.size() == 1) return;
    } else if (!isValInArray(value, this->allowedValues)) handleError(CDRES_SYNTAX_NOT_ALLOWED, "setTokenType+allowedValues");

    if      (value == "public")    this->setTokenTypeAccess(PUBLIC);
    else if (value == "private")   this->setTokenTypeAccess(PRIVATE);
    else if (value == "internal")  this->setTokenTypeAccess(INTERNAL);
    else if (value == "protected") this->setTokenTypeAccess(PROTECTED);

    else if (value == "trait")     this->setTokenTypeOOP(TRAIT);
    else if (value == "class")     this->setTokenTypeOOP(CLASS);
    else if (value == "impl")      this->setTokenTypeOOP(IMPL);

    else if (value == "func") {
        Token* func = this->currToken->add(FUNC);
        
        bool funcIsOOP = isOOP(this->currToken->type);
        bool funcHasAccess = isAccess(this->currToken->children.front()->type);
        if (funcIsOOP && funcHasAccess) {
            Token* access = this->currToken->children.front();

            this->currToken->cutToChild(access, func);
            setChild(this->currToken, func);
        } else if (funcIsOOP && !funcHasAccess) {
            Token* access = this->currToken->parent->children[0];

            setChild(this->currToken, func);
            this->currToken->add(access->type);
        } else if (!funcIsOOP && funcHasAccess) {
            handleError(CDRES_SYNTAX_NOT_ALLOWED, "setTokenType+funcCheck");
        } else {
            setChild(this->currToken, func);
            this->currToken->add(PUBLIC);
        }

        this->couldBeOpFunc = true;
        this->compareAgainst = func;
        this->allowedValues = { "__str" };
    }

    else if (value == "<") {
        if (this->currToken->parent->type == IF) {
            this->currToken->add(LT);
            return;
        }

        addSetChild(this->currToken, GENERIC_DEF);

        this->compareAgainst = __cToken; // "return" token for addSetChild
        this->allowedValues = { "__str", "&" };
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
    } else if (value == "(") {
        if (this->currToken->type == FUNC) {
            addSetChild(this->currToken, PARAMETER_DEF);
            this->compareAgainst = __cToken;
        } else {
            addSetChild(this->currToken, BEFORE_DEF);
            this->compareAgainst = __cToken;
        }
        this->allowedValues = { "__str", ")" };
    } else if (value == "{") {
        TokenType type = this->currToken->type;

        switch (type) {
            case TRAIT: {
                addSetChild(this->currToken, TRAIT_DEF);
                this->allowedValues = {
                    "}", "func", "var",
                    "public", "private", "internal", "protected"
                };
            } break;
            case CLASS: {
                addSetChild(this->currToken, CLASS_DEF);
                this->allowedValues = {
                    "}", "func", "var",
                    "public", "private", "internal", "protected"
                };
            } break;
            case IMPL:  {
                addSetChild(this->currToken, IMPL_DEF);
                this->allowedValues = {
                    "}", "func",
                    "public", "private", "internal", "protected"
                };
            } break;
            case FUNC:  {
                addSetChild(this->currToken, FUNCTION_DEF);
                this->allowedValues = {
                    "__str", "}", "var", "self",
                    "if", "while", "for", "ret", "switch", "const"
                };
            } break;
            default: handleError(CDRES_SYNTAX_NOT_DEFINED, "setTokenType+{check");
        }

        this->baseTokens.push_back(this->currToken);
    } else if (!isValInArray("__str", prevAV)) {
        handleError(CDRES_SYNTAX_NOT_DEFINED, "setTokenType+elseValue");
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
        if (sep & 0b10) {
            this->setToken(value);
        } else if (sep == 0b01 || (this->couldBeOpFunc && isValInArray(value, funcNames))) {
            this->setToken(value);
            this->setToken(currChar);
        } else {
            value.push_back(currChar);
        }
    }

    if (value.size() > 0) {
        this->badName = value;
        handleError(CDRES_SYNTAX_NOT_ALLOWED, "determineTokenLine");
    }
}

void Tokenizer::handleError(Result res, std::string func) {
    if (res == CDRES_OK) return;

    switch (res) {
        case CDRES_ARGS:
            std::cout << "Error: Invalid arguments" << std::endl;
            break;
        case CDRES_FILE:
            std::cout << "Error: Invalid file" << std::endl;
            break;
        case CDRES_SYNTAX_NOT_ALLOWED:
            std::cout << "Error: Syntax not allowed: \"" << this->badName << "\"" << std::endl;
            break;
        case CDRES_SYNTAX_NOT_DEFINED:
            std::cout << "Error: Syntax not defined: \"" << this->badName << "\"" << std::endl;
            break;
        case CDRES_UNIMPLEMENTED:
            std::cout << "Error: Unimplemented feature: " << this->badToken << std::endl;
            break;
        default:
            break;
    }
    
    std::cout << "@" << func << std::endl; 
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
    clrscrn();
    std::cout << "================" << std::endl;
    debugPrint(this->baseTokens[0]);
}