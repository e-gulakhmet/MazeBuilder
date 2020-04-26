#include <iostream>
#include <cstdlib>

#include "src/maze_gen.h"

int main(int, char**) {
    
    Field field(8, 8, 3, 2);

    // Инициализация генератора случайных чисел
    std::srand(0);

    field.trace_route();

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Cell cell = field.get_cell(x, y);
            switch (cell.type()) {
                case Cell::ctNormal: 
                    if (cell.path() != nullptr) // TODO: Измить условие, чтобы бьла возможность определять к какому пути принадлежит ячейка
                        std::cout << "1 ";
                    else
                        std::cout << "N ";
                    break;

                case Cell::ctStart: 
                    std::cout << "S ";
                    break;

                case Cell::ctFinish: 
                    std::cout << "F ";
                    break;
            }
        }
        std::cout << '\n';
    }

    return 0;
}
