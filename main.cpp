#include <iostream>
#include <cstdlib>

#include "src/maze_gen.h"

int main(int, char**) {
    
    Field field(8, 8, 3, 2);

    // Инициализация генератора случайных чисел
    std::srand(2);

    field.trace_route();

    std::cout << "Routes created.\n";

    std::cout << std::string(field);

    return 0;
}
