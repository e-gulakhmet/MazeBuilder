#include <cassert>

#include "maze_gen.h"



Field::Field(int w, int h, int start, int finish)
    : w_(w)
    , h_(h)
    , start_(start)
    , finish_(finish)
{
    bool walls[4];
    for (int y = 0; y < h_; y++)
        for (int x = 0; x < w_; x++) {
            walls[0] = (y == 0);
            walls[1] = (x == w_ - 1);
            walls[2] = (y == h_ - 1);
            walls[3] = (x == 0);
            Cell::CellType c_type;
            if (x == 0 && y == start_) 
                c_type = Cell::ctStart;
            else if (x == w_ - 1 && y == finish_)
                c_type = Cell::ctFinish;
            else
                c_type = Cell::ctNormal;

            Cell cell(x, y, c_type, walls);
            cells_.push_back(cell);
        }
}




Cell& Field::get_cell(int x, int y) {
    assert(x >= 0 && y >= 0 && x < w_ && y < h_);

    return cells_[y * w_ + x];
}



bool Field::trace_route() {
    // Создать путь
    Path path(this, Path::ptMain, &get_cell(0, start_));
    if (!path.create()) {
        std::cout << "Path could not be created.\n";
        return false;
    }
    else {
        pathes_.push_back(path);
    }

    // Создать ветвление
    // Зачистить непривязанные ячейки

    return true;
}



bool Path::create() {
    int dir;
    // Создаем путь
    // 1. Начинаем со стартовой ячейки, устанавливаем её первым элементом пути.
    Cell* curr = cells_[0];
    curr->set_path(this);

    while (true) {
        std::cout << "\n";
        std::cout << "walls_info: ";
        for (int wall = 0; wall <= 3; wall++) {
            std::cout << curr->walls(static_cast<Cell::CellDirection>(wall)) << " ";
        }
        std::cout << "\n";
        
        // 3. Проверяем все направления ячейки:
        //   1. Если направление указывает на ячейку, которая принадлежит пути, то ставим стену между ячейками и устанавливаем в обоих ячейках данное направление, как не доступное.
        // Проверяем верхнюю ячейку, получаем из field ячейку, у которой y текущей ячейки больше на 1
        std::cout << "closing walls: ";
        if (!curr->walls(Cell::cdTop) && field_->get_cell(curr->x(), curr->y() - 1).path() != nullptr) {
            curr->set_wall(Cell::cdTop, true);
            std::cout << "close_top_wall ";
        }
        // Проверяем правую ячейку, получаем из field ячейку, у которой x текущей ячейки больше на 1
        if (!curr->walls(Cell::cdRight) && field_->get_cell(curr->x() + 1, curr->y()).path() != nullptr) {
            curr->set_wall(Cell::cdRight, true);
            std::cout << "close_right_wall ";
        }
        // Проверяем нужнюю ячейку, получаем из field ячейку, у которой y текущей ячейки меньше на 1
        if (!curr->walls(Cell::cdBottom) && field_->get_cell(curr->x(), curr->y() + 1).path() != nullptr) {
            curr->set_wall(Cell::cdBottom, true);
            std::cout << "close_bottom_wall ";
        }
        // Проверяем левую ячейку, получаем из field ячейку, у которой x текущей ячейки меньше на 1
        if (!curr->walls(Cell::cdLeft) && field_->get_cell(curr->x() - 1, curr->y()).path() != nullptr) {
            curr->set_wall(Cell::cdLeft, true);
            std::cout << "close_left_wall";
        }
        std::cout << "\n";

        //   2. Если достигнута ячейка выхода, завершаем построение пути.
        if (curr->type() == Cell::ctFinish)
            break; 

        //   3. Если нет доступных направлений, то убираем у ячейки принадлежность к пути, возвращаемся на предыдущую ячейку и указываем данное направление как не доступное.
        if (curr->walls(Cell::cdTop) && curr->walls(Cell::cdRight) && curr->walls(Cell::cdBottom) && curr->walls(Cell::cdLeft)) {
            cells_.pop_back();
            curr->remove_path();
            // TODO: Поставить стену между ячейками
            curr = cells_.back();
            curr->set_wall(static_cast<Cell::CellDirection>(dir), true);
            continue;
        }

        while (true) {
            // 4. Случайно выбираем направление следующего шага из доступных.
            dir = rand() % 10 - 6; // TODO: Ограничить функцию rand от 0 до 3
            while (dir < 0) {
                dir = rand() % 10 - 6;
            }
            std::cout << "random_direction: " << dir << ", state: " << (curr->walls(static_cast<Cell::CellDirection>(dir)) ? "close" : "open") << "\n";

            // 5. Проверяем направление ячейки на доступность:
            //   1. Если направление доступно, указываем, то переходим к ячейке по выбранному направлению, указываем, что она принадлежит пути и повторяем шаг 3.3.
            std::cout << "sellecting_new_cell: ";
            if (!(curr->walls(static_cast<Cell::CellDirection>(dir)))) {
                switch (static_cast<Cell::CellDirection>(dir)) {
                    case Cell::cdTop:
                        std::cout << "up\n";
                        curr = &(field_->get_cell(curr->x(), curr->y() - 1));
                        cells_.push_back(curr);
                        curr->set_path(this);
                        break;

                    case Cell::cdRight:
                        std::cout << "right\n";
                        curr = &(field_->get_cell(curr->x() + 1, curr->y()));
                        cells_.push_back(curr);
                        curr->set_path(this);
                        break;

                    case Cell::cdBottom:
                        std::cout << "bottom\n";
                        curr = &(field_->get_cell(curr->x(), curr->y() + 1));
                        cells_.push_back(curr);
                        curr->set_path(this);
                        break;

                    case Cell::cdLeft:
                        std::cout << "left\n";
                        curr = &(field_->get_cell(curr->x() - 1, curr->y()));
                        cells_.push_back(curr);
                        curr->set_path(this);
                        break;             
                }
                std::cout << "\n";
                break;
            }
            std::cout << "\n";
            //   2. Иначе переходим к 3.4 шагу.
        }
    }


    return true;
}


