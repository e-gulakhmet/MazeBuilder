#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <iostream>
#include <string>

#include "../lib/bitmap/bitmap_image.hpp"
#include "maze_gen.h"



class Presenter{
    public:
        Presenter(Field& field);

        void bitmap(std::string file_name, int file_w, int file_h); // Создаем графический файл
        void debug(int level); // Выводим информацию о поле и т.д.
    
    private:
        Field& field_;

        void field_debug();
        void path_debug(Path& path);
        void fork_debug();
};




#endif // _OUTPUT_H_