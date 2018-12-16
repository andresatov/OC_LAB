//
// Created by andrey on 06.11.18.
//

#ifndef LAB5_OC_ACTION_H
#define LAB5_OC_ACTION_H

#define WINVER 0x0500


#include <functional>
#include <map>
#include "Action.h"
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <bitset>

class Action {
public:
    Action();

    std::function<void()> getAction(int num);

private:
    std::map<int, std::function<void()>> actionList;

    void winAPI_calculation_PI();

    void openMP_calculation_PI();
    volatile int first_free_block = 0;
    int block_size;
    std::vector<infoThread> piThreads;
    volatile int number_blocks;

    int calculation(LPVOID lpParameter);

};


#endif //LAB5_OC_ACTION_H
