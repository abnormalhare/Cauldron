#include "build/tokenizer.hpp"

Tokenizer::Tokenizer(std::ifstream& file) : file(file) {
    this->nodeStruct = std::make_shared<Node>();
    this->nodeStruct->parent = nullptr;
    this->nodeStruct->tokens = {};
    this->nodeStruct->name = "";
    this->nodeStruct->children = {};
}

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

bool Tokenizer::isTokenTypeValid(std::string value, Result& res) {
    if (this->allowedWords[0] == "__any" && this->isNameComplete) {
        std::cout << value << std::endl;

        if (value.size() == 0) {
            res = CDRES_SYNTAX;
        }

        this->isNameComplete = false;
        if (this->nodeStruct->tokens[1] == FUNC) {
            if (std::find(funcOpNames.begin(), funcOpNames.end(), value) == funcOpNames.end()) {
                this->nodeStruct->tokens[1] = OP_FUNC;
            }
        }
        if (std::find(disallowedNames.begin(), disallowedNames.end(), value) != disallowedNames.end()) {
            res = CDRES_SYNTAX;
        }

        this->nodeStruct->name = value;
        Node node = {this->nodeStruct, {}, {}};
        std::shared_ptr<Node> snode = std::make_shared<Node>(node);
        this->nodeStruct->children.push_back(snode);
    }

    if (!std::count(allowedWords.begin(), allowedWords.end(), value)) {
        return false;
    }
    std::cout << value << std::endl;

    if      (value == "public") this->setCurrNodeAccess(PUBLIC);
    else if (value == "private") this->setCurrNodeAccess(PRIVATE);
    else if (value == "protected") this->setCurrNodeAccess(PROTECTED);
    else if (value == "internal") this->setCurrNodeAccess(INTERNAL);
    else if (value == "class") {
        this->nodeStruct->tokens.push_back(CLASS);
        this->allowedWords = {"__any"};
        this->isNameReady = true;
    } else if (value == "trait") {
        this->nodeStruct->tokens.push_back(TRAIT);
        this->allowedWords = {"__any"};
        this->isNameReady = true;
    }

    return true;
}

bool isSeparator(char value) {
    return value == ';' || value == ',' || value == ':' || value == '.'
        || value == '{' || value == '}'
        || value == '(' || value == ')'
        || value == '[' || value == ']'
        || value == '<' || value == '>';
}

bool isSeparatorType(TokenType type) {
    return type == END || type == ITEM_SEPARATOR || type == TYPE_IDENT || type == MODULE_ACCESS
        || type == TYPE_DEF || type == FUNC_OP || type == PARAMETER_DEF
        || type == FUNCTION_DEF || type == CLASS_DEF || type == TRAIT_DEF || type == IMPL_DEF
        || type == COMMENT_DEF;
}

void Tokenizer::getToken(std::string value, Result& res) {
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
            
            tokenValue = "";
        }
    }

    if (tokenValue != "") {
        res = CDRES_SYNTAX;
    }
}

void printNode(std::shared_ptr<Node> node) {
    std::cout << "Node: " << node->name << std::endl;
    std::cout << "Tokens: " << std::endl;
    for (auto& token : node->tokens) {
        std::cout << "| " << printNames[token] << std::endl;
    }
}

void debugPrint(std::shared_ptr<Node> node) {
    // print all nodes
    printNode(node);
    for (auto& child : node->children) {
        debugPrint(child);
    }
};

void Tokenizer::handleError(Result res) {
    switch (res) {
        case CDRES_ARGS:
            std::cout << "Error: Invalid arguments" << std::endl;
            break;
        case CDRES_FILE:
            std::cout << "Error: Invalid file" << std::endl;
            break;
        case CDRES_SYNTAX:
            std::cout << "Error: Invalid syntax: " << printNames[this->nodeStruct->tokens.back()] << std::endl;
            break;
        default:
            break;
    }
}

void Tokenizer::tokenize() {
    Result res;

    std::cout << "Tokenizing file..." << std::endl;
    // while (!this->file.eof()) {
        std::string line;
        std::getline(this->file, line);
        this->getToken(line, res);

        while (this->nodeStruct->parent != nullptr) {
            this->nodeStruct = this->nodeStruct->parent;
        }
        debugPrint(this->nodeStruct);
    // }
}