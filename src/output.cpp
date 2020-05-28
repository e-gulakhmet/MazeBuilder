#include "output.h"


Presenter::Presenter(Field& field, std::string file_name, int file_w, int file_h)
    : field_(field)
    , name_(file_name)
    , w_(file_w)
    , h_(file_h)
{

}


void Presenter::bitmap(bool path_light) {
    // Отображаем информацию в файле
    cartesian_canvas canvas(w_, h_);
    // Устанавливаем цвет фона
    canvas.image().clear(0);    

    canvas.pen_width(3);
    // Рисуем поле
    int deltas[4][4] = {
        {0, 0, 1, 0},
        {1, 0, 1, 1},
        {0, 1, 1, 1},
        {0, 0, 0, 1}
    };
    // Проходимся по всем ячейкам пути
    for (int y = 0; y < field_.get_height(); y++)
        for (int x = 0; x < field_.get_width(); x++) {
            Cell& cell = field_.get_cell(x, y);
            // Если подсветка пути включена
            if (path_light) {
                if (cell.path() == &(field_.get_path()))
                    canvas.pen_color(100, 100, 0);
                else if (cell.path() != nullptr && cell.path() != &(field_.get_path()))
                    canvas.pen_color(100, 0, 0);

                canvas.fill_rectangle(canvas.min_x() + cell.x() * w_/ field_.get_width() + w_ / field_.get_width() / 3,
                                      canvas.min_y() + cell.y() * h_/ field_.get_height() + h_ / field_.get_height() / 3,
                                      canvas.min_x() + (cell.x() + 1) * w_/ field_.get_width() - w_ / field_.get_width() / 3,
                                      canvas.min_y() + (cell.y() + 1) * h_/ field_.get_height() - h_ / field_.get_height() / 3);
            }

            canvas.pen_color(255, 255, 255);
            for (int w = 0; w < 4; w++) {
                // Если по выбранному направлнию ячейка имеет стену
                if (cell.walls(static_cast<Cell::CellDirection>(w))){
                    // Отображаем стены с нужных сторон
                    canvas.line_segment(canvas.min_x() + (cell.x() + deltas[w][0]) * w_/ field_.get_width(),
                                        canvas.min_y() + (cell.y() + deltas[w][1]) * h_ / field_.get_height(),
                                        canvas.min_x() + (cell.x() + deltas[w][2]) * w_/ field_.get_width(),
                                        canvas.min_y() + (cell.y() + deltas[w][3]) * h_ / field_.get_height());
                }
            }
            // Ячейки старта и финиша отображаем кругами
            if (cell.type() == Cell::ctStart || cell.type() == Cell::ctFinish) {
                canvas.circle(canvas.min_x() + cell.x() * w_/ field_.get_width() + (w_/ field_.get_width() / 2),
                                canvas.min_y() + cell.y() * h_ / field_.get_height() + (h_ / field_.get_height() / 2),
                                w_/ field_.get_width() / 2);
            }
        }

    // Сохраняем файл
    canvas.image().vertical_flip();
    canvas.image().save_image(name_);
}



void Presenter::debug(int level) {
    switch (level) {
        case 1:
            // Выводим информацию о поле
            std::cout << "Field:" << "\n";
            field_debug();
            break;

        case 2:
            // Выводим информацию о пути
            std::cout << "Path:" << "\n";
            path_debug(field_.get_path());
            break;
        
        case 3:
            // Выводим информацию о форках
            std::cout << "Forks:" << "\n";
            fork_debug();
            break;

        case 4:
            // Выводим всю информацию
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
    
    std::cout << "widht: " << field_.get_width() << " height: " << field_.get_height() << "\n";
    std::cout << "cell_count: " << field_.get_width() * field_.get_height() << "\n";
}



void Presenter::path_debug(Path& path) {
    for (int i = 0; i < path.get_cells_size(); i++) {
        std::cout << i << " @ (";
        std::cout << path.get_cell(i)->x() << ',' << path.get_cell(i)->y() << ")\n";
    }
    
    std::cout << "cell_count: " << path.get_cells_size() << "\n";
}



void Presenter::fork_debug() {
    for (int i = 0; i < field_.get_forks_size(); i++){
        path_debug(field_.get_fork(i));
        std::cout << "\n";
    }
}