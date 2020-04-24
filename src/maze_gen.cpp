#include <cassert>
#include <algorithm>

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



int Field::get_cell_pos(int x, int y) { // Получение позиции в ячейки в пути
    assert(x >= 0 && y >= 0 && x < w_ && y < h_);

    // Получить путь, которому принадлежит ячейка
    Path* path = cells_[y * w_ + x].path();
    if (path == nullptr)
        return -1;

    return path->get_cell_id(&(cells_[y * w_ + x]));
}



bool Path::create() {
    int dir;
    int deltas[4][2] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };
    std::vector<int> free_dirs;
    // Создаем путь
    // 1. Начинаем со стартовой ячейки, устанавливаем её первым элементом пути.
    Cell* curr = cells_[0];
    curr->set_path(this);

    while (true) {
        free_dirs.clear();
        std::cout << "\n";
        std::cout << "walls_info: ";
        for (int wall = 0; wall <= 3; wall++) {
            std::cout << curr->walls(static_cast<Cell::CellDirection>(wall)) << " ";
        }
        std::cout << "\n";
        
        // 3. Проверяем все направления ячейки:
        //   1. Если направление указывает на ячейку, которая принадлежит пути, то ставим стену между ячейками и устанавливаем в обоих ячейках данное направление, как не доступное.
        // Проверяем верхнюю ячейку, получаем из field ячейку, у которой y текущей ячейки больше на 1
        for (int w = 0; w < 4; w++) {
            Cell::CellDirection cell_dir = static_cast<Cell::CellDirection>(w);
            if (curr->walls(cell_dir))
                continue;

            Cell& cell = field_->get_cell(curr->x() + deltas[w][0], curr->y() + deltas[w][1]);
            if (cell.path() != nullptr) {
                // Находим позицию текущей ячейки в векторе ячеек пути
                auto self = std::find(cells_.begin(), cells_.end(), curr);
                // Если мы не первая ячейка и проверяемая ячейка не является предудыщей, то ставим стенку
                if (self != cells_.begin() && self - 1 != std::find(cells_.begin(), cells_.end(), &cell)) {
                    curr->set_wall(cell_dir, true);
                    std::cout << "close_wall: " << w << '\n';
                }
            }
            else
                free_dirs.push_back(w);
        }

        //   2. Если достигнута ячейка выхода, завершаем построение пути.
        if (curr->type() == Cell::ctFinish)
            break; 

        //   3. Если нет доступных направлений, то убираем у ячейки принадлежность к пути, возвращаемся на предыдущую ячейку и указываем данное направление как не доступное.
        if (0 == free_dirs.size()) {
            cells_.pop_back();
            curr->remove_path();
            curr = cells_.back();
            curr->set_wall(static_cast<Cell::CellDirection>(dir), true);
            continue;
        }


        // 4. Случайно выбираем направление следующего шага из доступных.
        std::cout << "sellecting_new_cell: ";
        dir = free_dirs[rand() % free_dirs.size()];
        std::cout << dir << '\n';
        curr = &(field_->get_cell(curr->x() + deltas[dir][0], curr->y() + deltas[dir][1]));
        curr->set_path(this);
        cells_.push_back(curr);

        std::cout << "\n";

        //   2. Иначе переходим к 3.4 шагу.
    }


    return true;
}



int Path::get_cell_id(Cell* cell) {
    // Получаем итератор ячейки в пути
    auto cpos = std::find(cells_.begin(), cells_.end(), cell);
    // Если ячейка не найдена
    if (cpos == cells_.end()) {
        std::cout << "cell not found\n";
        return -1;
    }
    
    // Возвращаем разность между началом списка ячеек и интератором ячейки в пути
    return std::distance(cells_.begin(), cpos);
}