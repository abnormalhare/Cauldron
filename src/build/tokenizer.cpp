#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::ifstream& file) : file(file) {
    // create root node THAT SHOULD NEVER HAVE TOKENS OR A NAME
    this->nodeStruct = this->root = std::make_shared<Node>();
    this->nodeStruct->parent = nullptr;
    this->nodeStruct->tokens = {BAD_TOKEN};
    this->nodeStruct->name = "";

    // create first child node
    this->nodeStruct = this->createChildNode();
}

std::shared_ptr<Node> Tokenizer::createChildNode() {
    // create a node with current node as its parent
    Node node = {this->nodeStruct, {}, {}};
    // create a shared pointer to the node
    std::shared_ptr<Node> snode = std::make_shared<Node>(node);
    // add the node to the children of the current node
    this->nodeStruct->children.push_back(snode);
    return snode;
}

void Tokenizer::nsAdd(TokenType token) {
    this->nodeStruct->tokens.push_back(token);
}

void Tokenizer::nsSetParent() {
    this->nodeStruct = this->nodeStruct->parent;
}

void Tokenizer::setCurrNodeAccess(TokenType tokenType) {
    // oop words allowed
    this->allowedWords = {
        "class", "struct", "enum", "trait", "type"
    };

    if (this->nodeStruct->parent == this->root) {
        this->nsAdd(tokenType);
    } else {
        // if within OOP, add more allowed words
        // we shouldnt need to wory about functions and variables
        // outside of OOP because it should otherwise be procedural (and therefore public)
        this->allowedWords.push_back("func");
        this->allowedWords.push_back("var");

        this->nsAdd(tokenType);
    }
}

void Tokenizer::setCurrNodeStructure(TokenType tokenType) {
    // if we define a structure without access identified, it
    // should be private by default (like other languages)
    if (this->nodeStruct->tokens.size() == 0) {
        this->nsAdd(PRIVATE);
    }
    this->nsAdd(tokenType);
    this->allowedWords = {"__str"};
    this->isNameReady = true;
}

void Tokenizer::setCurrNodeFunction(TokenType tokenType) {
    // if we havent defined an access
    if (this->nodeStruct->tokens.size() == 0) {
        // if the function is defined outside of an OOP structure
        if (this->nodeStruct->parent == this->root) {
            // use procedural
            this->nsAdd(PUBLIC);
        } else {
            // otherwise, use the parent's access
            this->nsAdd(currParentAccess);
        }
    }
    this->nsAdd(tokenType);
    this->allowedWords = {"__str"};
    this->isNameReady = true;
}

void Tokenizer::setCurrNodeCompare(TokenType tokenType) {
    // if we are comparing outside an if statement, return
    std::shared_ptr<Node> node;
    for (node = this->nodeStruct; node != nullptr; node = node->parent) {
        if (node->tokens.size() > 0 && (node->tokens.at(0) == IF || node->tokens.at(0) == LAMBDA)) break;
    }
    if (node == nullptr) return;

    this->nsAdd(tokenType);
    this->allowedWords = {"__int", "="};
    this->isNameReady = true;
}

bool Tokenizer::isValueName(std::string value, Result& res) {
    if (this->allowedWords[0] != "__str" || !this->isNameComplete) return false;

    if (value.size() == 0) {
        this->badName = value;
        res = CDRES_SYNTAX;
        handleError(res);
    }

    this->isNameComplete = false;
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
        this->nodeStruct = createChildNode();
        this->allowedWords = { "<", "{" };
        break;
    
    case CLASS:
        this->nodeStruct = createChildNode();
        this->allowedWords = { "{" };
        break;
    
    case FUNC:
    case OP_FUNC:
        this->nodeStruct = createChildNode();
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
        this->nsSetParent();
        if (currParentName == "params") {
            this->allowedWords = { ":" };
        } else {
            this->allowedWords = { ",", "]" };
        }
        break;

    case TYPE_IDENT:
        this->nsSetParent();
        this->allowedWords = { ",", ")" };
        break;
    
    default:
        this->badToken = prevToken;
        res = CDRES_UNIMPLEMENTED;
        break;
    }

    return true;
}

bool Tokenizer::isTokenTypeValid(std::string value, Result& res) {
    if (value == "") return true;

    if (this->isValueName(value, res)) {
        handleError(res);
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

    else if (value == "<") { this->setCurrNodeCompare(LT);
        if (isOOP(currParentStructure)) {
            this->nsAdd(TYPE_DEF);
            this->allowedWords = {"__str"};
            this->isNameReady = true;
        }
    } else if (value == ">") { this->setCurrNodeCompare(GT);
        if (isOOP(currParentStructure)) {
            this->nsSetParent();
            this->allowedWords = {"{"};
        }
    } else if (value == "{") {
        this->nodeStruct = createChildNode();
        this->allowedWords = { "func", "impl" };
    } else if (value == "}") {
        while (this->nodeStruct->tokens.size() == 0) {
            this->nsSetParent();
        }
        while (this->nodeStruct != nullptr && (!isOOP(currStructure) && !isFunc(currStructure))) {
            this->nsSetParent();
        }
        this->nsSetParent();
        this->nodeStruct = createChildNode();

        allowedWords = baseAllowedTokens;
    } else if (value == "(") {
        this->nodeStruct->name = "params";
        this->nsAdd(PARAMETER_DEF);

        this->nodeStruct = createChildNode();
        this->nsAdd(PARAMETER_DEF);
        this->allowedWords = { "__str", ")" };
        this->isNameReady = true;
    } else if (value == ")") {
        while (!isOOP(currStructure) && !isFunc(currStructure)) {
            this->nsSetParent();
        }
        if (currParentStructure == TRAIT) {
            this->allowedWords = {";"};
        } else {
            this->allowedWords = {":", "{"};
        }
    } else if (value == ":") {
        this->nodeStruct = createChildNode();
        this->nodeStruct->name = "type";
        this->nsAdd(TYPE_IDENT);
        this->allowedWords = { "__str" };
        this->isNameReady = true;
    } else if (value == ",") {
        this->nsSetParent();
        this->nodeStruct = createChildNode();
        if (currParentName == "params") this->nsAdd(PARAMETER_DEF);
        this->allowedWords = { "__str" };
        this->isNameReady = true;
    } else if (value == ";") {
        while (this->nodeStruct != nullptr && (!isOOP(currStructure) || currStructure == FUNC)) {
            this->nsSetParent();
        }
        this->nodeStruct = createChildNode();

        allowedWords = baseAllowedTokens;
    }

    return true;
}

void Tokenizer::getToken(std::string value) {
    Result res = CDRES_OK;
    std::string tokenValue;

    for (size_t index = 0; index < value.size(); index++) {
        // if its a space, ignore it
        if (isspace(value[index])) continue;
        // if its a separator, and the name hasnt been run into
        if (isSeparator(value[index]) && this->isNameReady) {
            index--;
            this->isNameReady = false;
            this->isNameComplete = true;
        } else {
            tokenValue.push_back(value[index]);
        }

        if (this->isTokenTypeValid(tokenValue, res)) {
            this->callDebugPrint();
            tokenValue = "";
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

        // this->callDebugPrint();
    }
}

void Tokenizer::callDebugPrint() {
    std::cout << "================" << std::endl;
    debugPrint(this->root);
}