#include <iostream>
#include <cstdlib>
#include <cstring>

#include "src/maze_gen.h"
#include "lib/cxxopts/cxxopts.hpp"

auto cmd_parse(int argc, char** argv) {
    cxxopts::Options options("test", "'cxxopts' library test");

    options.add_options()
        ("help", "Print help")
        ("w,width", "Set field width", cxxopts::value<int>()->default_value("6"))
        ("h,height", "Set field height", cxxopts::value<int>()->default_value("6"))
        ("s,start", "Set start position", cxxopts::value<int>()->default_value("3"))
        ("f,finish", "Set finish position", cxxopts::value<int>()->default_value("2"))
        ("r,random", "Starting random seed", cxxopts::value<int>()->default_value("0"))
        ("debug", "Enable debug")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << "\n";
      exit(0);
    }

    int width = result["width"].as<int>();
    int height = result["height"].as<int>();
    int start_pos = result["start"].as<int>();
    int finish_pos = result["finish"].as<int>();
    int random_seed = result["random"].as<int>();

    std::cout << "w: " << width << "\n" \
                 "h: " << height << "\n" \
                 "s: " << start_pos << "\n" \
                 "f: " << finish_pos << "\n" \
                 "r: " << random_seed << "\n";

    return result;
}


int main(int argc, char** argv) {

    auto cmd_info = cmd_parse(argc, argv);    

    Field field(cmd_info["width"].as<int>(), cmd_info["height"].as<int>(), cmd_info["start"].as<int>(), cmd_info["finish"].as<int>());

    // Инициализация генератора случайных чисел
    std::srand(cmd_info["random"].as<int>());

    field.trace_route();

    std::cout << "Routes created.\n";

    std::cout << std::string(field);

    return 0;
}
