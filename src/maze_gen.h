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

        // Удаляем конструктор по-умолчанию
        Path() = delete;

        Path(Field *field, PathType type, Cell *first)
            : field_(field)
            , type_(type)
        {
            assert(field_ != nullptr);

            bind(first);
        };

        // Копирующий конструктор
        // Нельзя создать копию пути. Ячейка может принадлежить
        // только одному пути, поэтому копия не имеет связанных
        // с ней ячеек.
        Path(const Path& other) 
            : field_(other.field_)
            , type_(other.type_) 
        {};

        // Конструктор переноса
        Path(Path&& other) noexcept
            : field_(other.field_)
            , type_(other.type_)
            , cells_(std::move(other.cells_))
        {
            for (Cell* c : cells_ )
                c->set_path(this);
        };

        // Присваивание также создает путь, не имеющий принадлежащих 
        // ему ячеек.
        Path& operator=(const Path& other) {
            field_ = other.field_;
            type_ = other.type_;
            cells_.clear();

            return *this;
        };

        virtual bool create();
        bool create_fork();
        int get_cell_id(Cell* cell);
        void bind(Cell* cell);
        void unbind(Cell* cell);
        Cell* get_cell(int index) {return cells_[index];};
        
        operator std::string();

    protected:
        Field *field_;
        PathType type_;
        std::vector<Cell*> cells_;
};



class Fork: public Path{
    public:
        // Первая ячейка - ячейка пути от которой пошло ветвление, второая - первая ячейка ветвления
        Fork(Field *field, PathType type, std::pair<Cell*, Cell*> start)
        : Path(field, type, start.first)
        , path_cell_(start.first)
        {
            cells_.push_back(start.second);
        }

        typedef std::pair<Cell*, Cell*> ForkPair;

        bool create();
    
    private:
        Cell* path_cell_;

};



class Field {
    public:
        Field(int w, int h, int start, int finish);

        Cell &get_cell(int x, int y);
        bool trace_route();
        int get_cell_pos(int x, int y);
        void set_path(Path& path) {
            path_ = path;
        };
        
        Path& get_path(int index) {return path_;};
        void clear();

        operator std::string();
    
    private:
        int w_, h_;
        // start and finish position of the maze
        // start_ sits on the left side, finish_ on the right
        int start_, finish_; 
        
        std::vector<Cell> cells_;
        Path path_;
        std::vector<Fork> forks_;

        bool create_path();
};


#endif // _MAZE_GEN_H_