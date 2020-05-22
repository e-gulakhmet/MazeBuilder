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

    // Отображаем информацию в файле
    bitmap_image image(600, 600);
    image.set_all_channels(0, 0, 0);
    image_drawer draw(image);

    draw.pen_width(3);
    draw.pen_color(255, 255, 255);

    // Рисуем поле
    for (int y = 0; y < field.get_height(); y++) {
        for (int x = 0; x < field.get_width(); x++) {
            // draw.rectangle(x * image.width() / field.get_width(), y * image.height() / field.get_height(), (x + 1) * image.width() / 6, (y + 1) * image.height() / 6);
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

            }
            


    //  for (int y = 0; y < h_; y++) {
    //     for (int x = 0; x < w_; x++) {
    //         Cell& cell = cells_[y * w_ + x];
    //         switch (cell.type()) {
    //             case Cell::ctNormal:
    //                 if (cell.path() == nullptr)
    //                     ss << "N ";
    //                 else if (cell.path() == &(*path_))
    //                     // ss << pathes_[0].get_cell_id(&cell) % 10 << ' ';
    //                     ss << get_cell_pos(x, y) % 10 << ' ';
    //                     // ss << "* ";
    //                 else
    //                     ss << "F ";
    //                 break;
                
    //             case Cell::ctStart:
    //                 ss << "S ";
    //                 break;
                
    //             case Cell::ctFinish:
    //                 ss << "F ";
    //                 break;
    //         }
    //     }
    //     ss << "\n";
    // }
        }
    }


    image.save_image("image.bmp");


    return 0;
}
