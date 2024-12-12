#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::ifstream& file) : file(file) {
    // create root node THAT SHOULD NEVER HAVE TOKENS OR A NAME
    Node node = Node{nullptr, {BAD_TOKEN}, "", {}};
    this->nodeStruct = this->root = new Node(node);
    // create first child node
    this->nodeStruct = this->nodeStruct->newChild();
}

void Tokenizer::setCurrNodeAccess(TokenType tokenType) {
    // oop words allowed
    this->allowedWords = {
        "class", "struct", "enum", "trait", "type"
    };

    if (this->nodeStruct->parent == this->root) {
        this->nodeStruct->add(tokenType);
    } else {
        // if within OOP, add more allowed words
        // we shouldnt need to wory about functions and variables
        // outside of OOP because it should otherwise be procedural (and therefore public)
        this->allowedWords.push_back("func");
        this->allowedWords.push_back("var");

        this->nodeStruct->add(tokenType);
    }
}

void Tokenizer::setCurrNodeStructure(TokenType tokenType) {
    // if we define a structure without access identified, it
    // should be private by default (like other languages)
    if (this->nodeStruct->tokens.size() == 0) {
        this->nodeStruct->add(PRIVATE);
    }
    this->nodeStruct->add(tokenType);
    this->allowedWords = {"__str"};
}

void Tokenizer::setCurrNodeFunction(TokenType tokenType) {
    // if we havent defined an access
    if (this->nodeStruct->tokens.size() == 0) {
        // if the function is defined outside of an OOP structure
        if (this->nodeStruct->parent == this->root) {
            // use procedural
            this->nodeStruct->add(PUBLIC);
        } else {
            // otherwise, use the parent's access
            this->nodeStruct->add(currParentAccess);
        }
    }
    this->nodeStruct->add(tokenType);
    this->allowedWords = {"__str"};
}

void Tokenizer::setCurrNodeCompare(TokenType tokenType) {
    Node* node;
    // check to see if we are 
    for (node = this->nodeStruct; node != nullptr; node = node->parent) {
        if (node->tokens.size() > 0 && (node->tokens.at(0) == IF)) break;
    }
    if (node == nullptr) return;

    this->nodeStruct->add(tokenType);
    this->allowedWords = {"__int", "="};
}

void Tokenizer::setCurrNodeVar(std::string tokenName) {
    TokenType token;

    this->nodeStruct->tokens.pop_back();
         if (tokenName == "void") token = VOID;
    else if (tokenName == "s8") token = S8;
    else if (tokenName == "u8") token = U8_CHAR;
    else if (tokenName == "char") token = U8_CHAR;
    else if (tokenName == "s16") token = S16;
    else if (tokenName == "u16") token = U16;
    else if (tokenName == "s32") token = S32;
    else if (tokenName == "u32") token = U32;
    else if (tokenName == "s64") token = S64;
    else if (tokenName == "u64") token = U64;
    else if (tokenName == "f32") token = F32;
    else if (tokenName == "f64") token = F64;
    else if (tokenName == "f128") token = F128;
    else if (tokenName == "bool") token = BOOL;
    else if (tokenName == "string") token = STRING;
    else if (tokenName == "func") token = FUNC;
    else if (tokenName == "date") token = DATE;
    else if (tokenName == "array") token = ARRAY;
    else if (tokenName == "vararray") token = VARARRAY;
    else if (tokenName == "hashmap") token = HASHMAP;

    this->nodeStruct->add(token);
    this->allowedWords = { "=", ";" };
}

bool Tokenizer::isValueName(std::string value, Result& res) {
    if (!isValInArray("__str", this->allowedWords)) return false;
    if (isValInArray(value, this->allowedWords)) return false;

    if (value.size() == 0) {
        this->badName = value;
        res = CDRES_SYNTAX;
        handleError(res);
    }

    TokenType prevToken = this->nodeStruct->tokens.at(indexBackward(-1));
    TokenType typeToken = this->nodeStruct->tokens.at(0);
    if (prevToken != FUNC) {
        if (isValInArray(value, disallowedNames) || areSymbolsInVal(value, disallowedSymbols)) {
            this->badName = value;
            res = CDRES_SYNTAX;
            handleError(res);
        }
    } else if (isValInArray(value, disallowedFuncOpNames)) {
        this->badName = value;
        res = CDRES_SYNTAX;
        handleError(res);
    } else if (isValInArray(value, funcOpNames)) {
        this->nodeStruct->tokens.at(indexBackward(-1)) = OP_FUNC;
    }

    this->nodeStruct->name = value;

    switch (prevToken) {
    case TRAIT:
        this->allowedWords = { "<", "{" };
        break;
    
    case CLASS:
        this->allowedWords = { "{" };
        break;
    
    case FUNC:
    case OP_FUNC:
        this->nodeStruct = this->nodeStruct->newChild();
        if (typeToken == OVERLOAD)
             this->allowedWords = { "{" };
        else this->allowedWords = { "(" };
        break;

    case TYPE_DEF:
        this->allowedWords = { ">" };
        break;

    case PARAMETER_DEF:
        this->allowedWords = { ":" };
        break;

    case ITEM_SEPARATOR:
        this->nodeStruct->tokens.pop_back();
        this->nodeStruct = this->nodeStruct->parent;
        if (currParentName == "params") {
            this->allowedWords = { ":" };
        } else {
            this->allowedWords = { ",", "]" };
        }
        break;

    case TYPE_IDENT:
        this->nodeStruct = this->nodeStruct->parent;
        if (currStructure == FUNC) {
            this->allowedWords = { "{" };
        } else {
            this->allowedWords = { ",", ")" };
        }
        break;

    case VAR:
        this->allowedWords = { ":" };
        break;
    
    default:
        this->badToken = prevToken;
        res = CDRES_UNIMPLEMENTED;
        break;
    }

    if (res != CDRES_OK) this->handleError(res);

    return true;
}

bool Tokenizer::isTokenTypeValid(std::string value, Result& res) {
    if (value == "") return true;

    if (isValInArray(value, baseAllowedTypes) && isValInArray("__str", this->allowedWords)) {
        this->setCurrNodeVar(value);
        return true;
    }

    if (this->isValueName(value, res)) {
        return true;
    }

    if (!std::count(allowedWords.begin(), allowedWords.end(), value)) {
        return false;
    }

         if (value == "public") this->setCurrNodeAccess(PUBLIC);
    else if (value == "private") this->setCurrNodeAccess(PRIVATE);
    else if (value == "protected") this->setCurrNodeAccess(PROTECTED);
    else if (value == "internal") this->setCurrNodeAccess(INTERNAL);

    else if (value == "class") this->setCurrNodeStructure(CLASS);
    else if (value == "trait") this->setCurrNodeStructure(TRAIT);
    else if (value == "struct") this->setCurrNodeStructure(STRUCT);
    else if (value == "enum") this->setCurrNodeStructure(ENUM);

    else if (value == "func") this->setCurrNodeFunction(FUNC);
    else if (value == "overload") this->setCurrNodeFunction(OVERLOAD);
    else if (value == "override") this->setCurrNodeFunction(OVERRIDE);

    else if (value == "var") {
        this->nodeStruct = this->nodeStruct->newChild();
        this->nodeStruct->add(VAR);
        this->allowedWords = { "__str" };
    } else if (value == "ret") {
        this->nodeStruct = this->nodeStruct->newChild();
        this->nodeStruct->add(RET);
        this->nodeStruct = this->nodeStruct->newChild();
        this->allowedWords = { "__str", "self", "(" };
    } else if (value == "self") {
        this->nodeStruct->add(SELF);
        this->allowedWords = { "__s", ".", "}" };
    } else if (value == "<") {
        this->nodeStruct = this->nodeStruct->newChild();
        if (isOOP(currParentStructure)) {
            this->nodeStruct->add(TYPE_DEF);
            this->allowedWords = {"__str"};
        } else {
            this->setCurrNodeCompare(LT);
        }
    } else if (value == ">") {
        if (isOOP(currParentStructure)) {
            this->nodeStruct = this->nodeStruct->parent;
            this->allowedWords = {"{"};
        } else {
            this->nodeStruct = this->nodeStruct->newChild();
            this->setCurrNodeCompare(GT);
        }
    } else if (value == "{") {
        if (currStructure == TRAIT) {
            this->nodeStruct = this->nodeStruct->newChild();
            this->allowedWords = { "func", "impl", "}" };
        } else if (currStructure == CLASS) {
            this->allowedWords = { "func", "impl", "var", "const", "}" };
        } else if (isFunc(currStructure)) {
            this->allowedWords = { "__str", "ret", "self", "var", "const", "}" };
        }
    } else if (value == "}") {
        while (this->nodeStruct->tokens.size() == 0) {
            this->nodeStruct = this->nodeStruct->parent;
        }
        while (this->nodeStruct != nullptr && (!isOOP(currStructure) && !isFunc(currStructure))) {
            this->nodeStruct = this->nodeStruct->parent;
        }
        this->nodeStruct = this->nodeStruct->parent;
        this->nodeStruct = this->nodeStruct->newChild();

        allowedWords = baseAllowedTokens;
    } else if (value == "(") {
        this->nodeStruct->name = "params";
        this->nodeStruct->add(PARAMETER_DEF);

        this->nodeStruct = this->nodeStruct->newChild();
        this->nodeStruct->add(PARAMETER_DEF);
        this->allowedWords = { "__str", ")" };
    } else if (value == ")") {
        while (!isOOP(currStructure) && !isFunc(currStructure)) {
            this->nodeStruct = this->nodeStruct->parent;
        }
        if (currParentStructure == TRAIT) {
            this->allowedWords = {";"};
        } else {
            this->allowedWords = {":", "{"};
        }
    } else if (value == ":") {
        this->nodeStruct = this->nodeStruct->newChild();
        this->nodeStruct->name = "type";
        this->nodeStruct->add(TYPE_IDENT);
        this->allowedWords = baseAllowedTypes;
        this->allowedWords.push_back("__str");
    } else if (value == ",") {
        this->nodeStruct = this->nodeStruct->parent;
        this->nodeStruct = this->nodeStruct->newChild();
        if (currParentName == "params") this->nodeStruct->add(PARAMETER_DEF);
        this->allowedWords = { "__str" };
    } else if (value == ";") {
        while (this->nodeStruct != nullptr && (!isOOP(currStructure) || currStructure == FUNC)) {
            this->nodeStruct = this->nodeStruct->parent;
        }
        this->nodeStruct = this->nodeStruct->newChild();

        allowedWords = baseAllowedTokens;
    } else if (value == "=") {
        int pos;
        std::vector<TokenType> currTokens = this->nodeStruct->tokens;

        auto it = std::find(currTokens.begin(), currTokens.end(), LT);
        if (it != currTokens.end()) {
            pos = std::distance(currTokens.begin(), it);
            this->nodeStruct->tokens.at(pos) = LE;
        }

        auto it2 = std::find(currTokens.begin(), currTokens.end(), GT);
        if (it2 != currTokens.end()) {
            pos = std::distance(currTokens.begin(), it2);
            this->nodeStruct->tokens.at(pos) = GE;
        }
    }

    return true;
}

void Tokenizer::getToken(std::string value) {
    Result res = CDRES_OK;
    std::string tokenValue;
    char currChar;

    for (size_t index = 0; index < value.size(); index++) {
        currChar = value[index];

        uint8_t sep = ((isspace(currChar) != 0) << 1) | isSeparator(currChar);
        if (sep == 0b00) { // is a standard character
            tokenValue.push_back(currChar);
        } else if (sep == 0b01) { // is a separator
            if (this->isTokenTypeValid(tokenValue, res)) {
                this->callDebugPrint();
                tokenValue = "";
                tokenValue.push_back(currChar);
                this->isTokenTypeValid(tokenValue, res);
                this->callDebugPrint();
                tokenValue = "";
            } else {
                res = CDRES_SYNTAX;
            }
        } else if (sep == 0b10 || sep == 0b11) { // is a space
            if (this->isTokenTypeValid(tokenValue, res)) {
                this->callDebugPrint();
                tokenValue = "";
            }
        }
    }

    if (tokenValue.size() > 0) {
        this->badName = tokenValue;
        res = CDRES_SYNTAX;
    }

    handleError(res);
}

void Tokenizer::handleError(Result res) {
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
    
    if (res != CDRES_OK) exit(res);
}

void Tokenizer::tokenize() {
    std::cout << "Tokenizing file..." << std::endl;
    while (!this->file.eof()) {
        std::string line;
        std::getline(this->file, line);
        this->getToken(line);
    }
}

void Tokenizer::callDebugPrint() {
    std::cout << "================" << std::endl;
    debugPrint(this->root);
}