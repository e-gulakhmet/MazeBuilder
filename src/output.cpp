#include "output.h"


Presenter::Presenter(Field& field)
    : field_(field)
{

}


void Presenter::bitmap(std::string file_name, int file_w, int file_h) {
    // Отображаем информацию в файле
    bitmap_image image(file_w, file_h);
    image.set_all_channels(0, 0, 0);
    image_drawer draw(image);

    draw.pen_width(3);
    draw.pen_color(255, 255, 255);

    // Рисуем поле
    int deltas[4][4] = {
        {0, 0, 1, 0},
        {1, 0, 1, 1},
        {0, 1, 1, 1},
        {0, 0, 0, 1}
    };
    for (int y = 0; y < field_.get_height(); y++)
        for (int x = 0; x < field_.get_width(); x++) {
            Cell& cell = field_.get_cell(x, y);
            for (int w = 0; w < 4; w++) {
                if (cell.walls(static_cast<Cell::CellDirection>(w))){
                    draw.line_segment((cell.x() + deltas[w][0]) * image.width() / field_.get_width(),
                                      (cell.y() + deltas[w][1]) * image.height() / field_.get_height(),
                                      (cell.x() + deltas[w][2]) * image.width() / field_.get_width(),
                                      (cell.y() + deltas[w][3]) * image.height() / field_.get_height());
                }

                if (cell.type() == Cell::ctStart || cell.type() == Cell::ctFinish) {
                    draw.circle(cell.x() * image.width() / field_.get_width() + (image.width() / field_.get_width() / 2),
                                cell.y() * image.height() / field_.get_height() + (image.height() / field_.get_height() / 2),
                                image.width() / field_.get_width() / 2);
                }
            }
        }

    image.save_image(file_name);
}



void Presenter::debug(int level) {
    switch (level) {
        case 1:
            field_debug();
            break;

        case 2:
            path_debug(field_.get_path());
            break;
        
        case 3:
            fork_debug();
            break;

        case 4:
            field_debug();
            std::cout << "\n";
            path_debug(field_.get_path());
            std::cout << "\n";
            fork_debug();                
            break;
    }
}




void Presenter::field_debug() {
    for (int y = 0; y < field_.get_height(); y++) {
        for (int x = 0; x < field_.get_width(); x++) {
            Cell& cell = field_.get_cell(x, y);
            switch (cell.type()) {
                case Cell::ctNormal:
                    if (cell.path() == nullptr)
                        std::cout << "N ";
                    else if (cell.path() == &(field_.get_path()))
                        // std::cout << pathes_[0].get_cell_id(&cell) % 10 << ' ';
                        std::cout << field_.get_cell_pos(x, y) % 10 << ' ';
                        // std::cout << "* ";
                    else
                        std::cout << "F ";
                    break;
                
                case Cell::ctStart:
                    std::cout << "S ";
                    break;
                
                case Cell::ctFinish:
                    std::cout << "F ";
                    break;
            }
        }
        std::cout << "\n";
    }
}



void Presenter::path_debug(Path& path) {
    for (int i = 0; i < path.get_cells_size(); i++) {
        std::cout << i << " @ (";
        std::cout << path.get_cell(i)->x() << ',' << path.get_cell(i)->y() << ")\n";
    }
}



void Presenter::fork_debug() {
    for (int i = 0; i < field_.get_forks_size(); i++){
        path_debug(field_.get_fork(i));
        std::cout << "\n";
    }
}