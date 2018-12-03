//
// Created by andrey on 02.12.18.
//

#include "view.h"

void outputMenu(const std::string &menu) {
    std::ifstream fileMenu(menu, std::ios::in | std::ios::binary);
    std::string tmp;
    while (getline(fileMenu, tmp)) std::cout << tmp << std::endl;
}

int inputChoice() {
    std::cout << ">> ";
    uint value;
    while (!(std::cin >> value) || (value > 13 ) || (value < 0)) {
        std::cout << "error" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << ">> ";
    }
    return value;
}

