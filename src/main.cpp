#include "main.hpp"

int main(int argc, char* argv[]) {
    char* cmd = argv[1];
    char* file = argv[2];

    if (cmd == "build") {
        const Parser parser = Parser(file);
        parser.parse();
    }

    return CDRES_OK;
}