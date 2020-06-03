#include <iostream>
#include <cstdlib>
#include <cstring>

#include "src/maze_gen.h"
#include "src/output.h"
#include "lib/cxxopts/cxxopts.hpp"
#include "lib/bitmap/bitmap_image.hpp"


// TODO: Добавить ограничения по минимальному размеру окна



auto cmd_parse(int argc, char** argv) {
    cxxopts::Options options("test", "'cxxopts' library test");

    options.add_options()
        ("help", "Print help")
        ("w,width", "Set field width", cxxopts::value<int>()->default_value("6"))
        ("h,height", "Set field height", cxxopts::value<int>()->default_value("6"))
        ("s,start", "Set start position", cxxopts::value<int>()->default_value("3"))
        ("f,finish", "Set finish position", cxxopts::value<int>()->default_value("2"))
        ("r,random", "Starting random seed", cxxopts::value<int>()->default_value("0"))
        ("debug", "Enable debug", cxxopts::value<int>()->default_value("0"))
        ("image_widht", "Set image file widht", cxxopts::value<int>()->default_value("600"))
        ("image_height", "Set image file height", cxxopts::value<int>()->default_value("600"))
        ("p, path", "Enable path and forks highlight", cxxopts::value<bool>()->default_value("false"))
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << "\n";
      exit(0);
    }

    return result;
}



int main(int argc, char** argv) {

    auto cmd_info = cmd_parse(argc, argv);    

    // Field field(cmd_info["width"].as<int>(), cmd_info["height"].as<int>(), cmd_info["start"].as<int>(), cmd_info["finish"].as<int>());
    Field field(10, 10, cmd_info["start"].as<int>(), cmd_info["finish"].as<int>());

    // Инициализация генератора случайных чисел
    std::srand(cmd_info["random"].as<int>());

    field.trace_route();

    Presenter present(field, "maze.bmp", 600, 600);

    present.debug(cmd_info["debug"].as<int>());
    std::cout << "\n";
    //present.bitmap(cmd_info["path"].as<bool>());
    present.bitmap(1);

    return 0;
}
