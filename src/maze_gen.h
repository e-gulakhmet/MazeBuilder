#ifndef _MAZE_GEN_H_
#define _MAZE_GEN_H_

#include <iostream>
#include <vector>
#include <cassert>

class Path;



class Cell {
    public:
        typedef enum {
            ctStart,
            ctFinish,
            ctNormal
        } CellType;

        typedef enum {
            cdTop,
            cdRight,
            cdBottom,
            cdLeft
        } CellDirection;

        Cell(int x, int y, CellType type, bool walls[4])
            : x_(x)
            , y_(y)
            , path_(nullptr)
            , type_(type)
        {
            for (int i = 0; i < 4; i++)
                walls_[i] = walls[i];
        };

        CellType type() const {return type_;};
        bool walls(CellDirection dir) {return walls_[dir];}
        int x() {return x_;};
        int y() {return y_;};
        Path* path() {return path_;};
        void set_wall(CellDirection dir, bool state) {walls_[dir] = state;};
        void set_path(Path* path=nullptr) {path_ = path;};

    private:
        int x_, y_;
        Path* path_;
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

        // Конструктор по-умолчанию
        Path() = delete;

        Path(Field *field, PathType type, Cell *first)
            : field_(field)
            , type_(type)
        {
            assert(field_ != nullptr);

            bind(first);
        };

        // Копирующий конструктор
        Path(const Path& other) 
            : field_(other.field_)
            , type_(other.type_)
        {
            std::copy(other.cells_.begin(), other.cells_.end(), cells_.begin());
        }

        // конструктор переноса
        Path(Path&& other) 
            : field_(other.field_)
            , type_(other.type_)
        {
            for (auto c : other.cells_)
                cells_.push_back(c);
        };

        Path& operator=(const Path& other) {
            field_ = other.field_;
            type_ = other.type_;
            cells_.clear();
            std::copy(other.cells_.begin(), other.cells_.end(), cells_.begin());

            return *this;
        };

        bool create();
        bool create_fork();
        int get_cell_id(Cell* cell);
        void bind(Cell* cell);
        void unbind(Cell* cell);
        
        operator std::string();

    private:
        Field *field_;
        PathType type_;
        std::vector<Cell*> cells_;
        typedef std::pair<Cell*, Path*> ForkStart;
};



class Field {
    public:
        Field(int w, int h, int start, int finish);


        Cell &get_cell(int x, int y);
        bool trace_route();
        int get_cell_pos(int x, int y);
        void add_path(Path& path) {
            std::cout << "Trying to add the new path\n";
            pathes_.push_back(std::move(path));
            std::cout << "New path added\n";
        };
        Path& get_path(int index) {return pathes_[index];};
        void clear();

        operator std::string();
    
    private:
        int w_, h_;
        // start and finish position of the maze
        // start_ sits on the left side, finish_ on the right
        int start_, finish_; 
        
        std::vector<Cell> cells_;
        std::vector<Path> pathes_;

        bool create_path();
};



#endif // _MAZE_GEN_H_