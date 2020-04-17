#include <iostream>

#include "src/maze_gen.h"

int main(int, char**) {
    
    Field field(6, 6, 3, 1);

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 6; x++) {
            Cell cell = field.get_cell(x, y);
            switch (cell.type()) {
                case Cell::ctNormal: 
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
        std::cout << "\n";
    }

    return 0;
}
