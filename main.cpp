#include <iostream>
#include <cstdlib>
#include <cstring>

#include "src/maze_gen.h"

void cmd_parse(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << " --help for help\n";
        return;
    }   

    if (strcmp(argv[1], "--help") == 0) {
        std::cout << "Usage: maze_builder.exe [OPTIONS]\n" \
                     "  OPTIONS are:\n" \
                     "    -help             see help\n" \
                     "    -r<number>        starting random seed\n" \
                     "    -v                verbose output\n" \
                     "    -o<text|graph>    output file text or png image\n";
    }

    std::vector<std::string> params;
    for (int p = 0; p < argc; p++)
        params.push_back(argv[p]);

    for (auto p : params)
        std::cout << p << '\n';
}


int main(int argc, char** argv) {

    cmd_parse(argc, argv);

    Field field(10, 10, 3, 2);

    // Инициализация генератора случайных чисел
    std::srand(2);

    field.trace_route();

    std::cout << "Routes created.\n";

    std::cout << std::string(field);

    return 0;
}
