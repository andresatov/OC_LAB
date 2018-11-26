//
// Created by andrey on 06.11.18.
//

#ifndef LAB3_OC_VIEW_H
#define LAB3_OC_VIEW_H

#include <iostream>
#include <fstream>
#include <filesystem>
namespace {
    void outputMenu(const std::string &menu) {
        std::ifstream fileMenu(menu, std::ios::in | std::ios::binary);
        std::string tmp;
        while (getline(fileMenu, tmp)) std::cout << tmp << std::endl;
    }

    int inputChoice() {
        std::cout << ">> ";
        int value;
        while (!(std::cin >> value)) {
            std::cout << "error" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << ">> ";
        }
        return value;
    }

    bool checkIsPath(std::filesystem::path path) {
        if (!std::filesystem::exists(path)) {
            std::cout << "Can't find file!" << path << std::endl;
            return false;
        }
        return true;
    }
}
#endif //LAB3_OC_VIEW_H