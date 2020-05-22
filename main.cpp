#include <iostream>
#include <cstdlib>
#include <cstring>

#include "src/maze_gen.h"
#include "lib/cxxopts/cxxopts.hpp"
#include "lib/bitmap/bitmap_image.hpp"



auto cmd_parse(int argc, char** argv) {
    cxxopts::Options options("test", "'cxxopts' library test");

    options.add_options()
        ("help", "Print help")
        ("w,width", "Set field width", cxxopts::value<int>()->default_value("6"))
        ("h,height", "Set field height", cxxopts::value<int>()->default_value("6"))
        ("s,start", "Set start position", cxxopts::value<int>()->default_value("3"))
        ("f,finish", "Set finish position", cxxopts::value<int>()->default_value("2"))
        ("r,random", "Starting random seed", cxxopts::value<int>()->default_value("0"))
        ("debug", "Enable debug", cxxopts::value<bool>()->default_value("false"))
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << "\n";
      exit(0);
    }

    return result;
}



void draw_bitmap(Field& field) {
    // Отображаем информацию в файле
    bitmap_image image(600, 600);
    image.set_all_channels(0, 0, 0);
    image_drawer draw(image);

    draw.pen_width(3);
    draw.pen_color(255, 255, 255);

    // Рисуем поле
    for (int y = 0; y < field.get_height(); y++) {
        for (int x = 0; x < field.get_width(); x++) {
            Cell& cell = field.get_cell(x, y);
            for (int w = 0; w < 4; w++) {
                if (cell.walls(static_cast<Cell::CellDirection>(w))){
                    switch (static_cast<Cell::CellDirection>(w)) {
                        case Cell::cdTop:
                            draw.line_segment(cell.x() * image.width() / field.get_width(), cell.y() * image.height() / field.get_height(),
                                                (cell.x() + 1) * image.width() / field.get_width(), cell.y() * image.height() / field.get_height());
                            break;
                        
                        case Cell::cdRight:
                            draw.line_segment((cell.x() + 1) * image.width() / field.get_width(), cell.y() * image.height() / field.get_height(),
                                              (cell.x() + 1) * image.width() / field.get_width(), (cell.y() + 1) * image.height() / field.get_height());
                            break;
                        
                        case Cell::cdBottom:
                            draw.line_segment(cell.x() * image.width() / field.get_width(), (cell.y() + 1) * image.height() / field.get_height(),
                                              (cell.x() + 1) * image.width() / field.get_width(), (cell.y() + 1) * image.height() / field.get_height());
                            break;

                        case Cell::cdLeft:
                            draw.line_segment(cell.x() * image.width() / field.get_width(), cell.y() * image.height() / field.get_height(),
                                              cell.x() * image.width() / field.get_width(), (cell.y() + 1) * image.height() / field.get_height());
                            break;
                    }
                }
                if (cell.type() == Cell::ctStart || cell.type() == Cell::ctFinish) {
                    draw.circle(cell.x() * image.width() / field.get_width() + (image.width() / field.get_width() / 2),
                                cell.y() * image.height() / field.get_height() + (image.height() / field.get_height() / 2),
                                image.width() / field.get_width() / 2);
                }
            }
        }
    }

    image.save_image("image.bmp");
}



int main(int argc, char** argv) {

    auto cmd_info = cmd_parse(argc, argv);    

    Field field(cmd_info["width"].as<int>(), cmd_info["height"].as<int>(), cmd_info["start"].as<int>(), cmd_info["finish"].as<int>());

    // Инициализация генератора случайных чисел
    std::srand(cmd_info["random"].as<int>());

    field.trace_route();

    if (cmd_info["debug"].as<bool>()) {
        std::cout << "Routes created.\n";
        std::cout << std::string(field);
    }
    
    draw_bitmap(field);

    return 0;
}
