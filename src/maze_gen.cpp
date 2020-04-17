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




Cell Field::get_cell(int x, int y) {
    assert(x >= 0 && y >= 0 && x < w_ && y < h_);

    return cells_[y * w_ + x];
}