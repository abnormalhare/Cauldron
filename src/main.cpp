#include "main.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Error: Not enough arguments\n");
        exit(CDRES_ARGS);
    }

    std::string cmd = argv[1];
    std::string file = argv[2];

    if (cmd == "build") {
        Parser parser = Parser(file);
        parser.parse();
    }

    return CDRES_OK;
}