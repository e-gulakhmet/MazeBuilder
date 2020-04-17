#ifndef _MAZE_GEN_H_
#define _MAZE_GEN_H_

#include <vector>


class Path;



class Cell {
    public:
        typedef enum {
            ctStart,
            ctFinish,
            ctNormal
        } CellType;

        Cell(int x, int y, CellType type, bool walls[4])
            : x_(x)
            , y_(y)
            , path_(nullptr)
            , type_(type)
            {
                for (int i = 0; i < 4; i++)
                    walls_[i] = walls[i];
            };

        CellType type() {return type_;};
    
    private:
        int x_, y_;
        Path *path_;
        CellType type_;
        bool walls_[4];
};



class Field;

class Path {
    public:
        typedef enum {
            ptMain,
            ptFork,
        } PathType;

        Path(Field *field, PathType type, Cell *first)
            : field_(field)
            , type_(type)
            {
                cells_.push_back(first);
            }

    private:
        Field *field_;
        PathType type_;
        std::vector<Cell*> cells_;
};



class Field {
    public:
        Field(int w, int h, int start, int finish);

        Cell get_cell(int x, int y);

    
    private:
        int w_, h_;
        // start and finish position of the maze
        // start_ sits on the left side, finish_ on the right
        int start_, finish_; 
        
        std::vector<Cell> cells_;
        std::vector<Path> pathes_;
};



#endif // _MAZE_GEN_H_