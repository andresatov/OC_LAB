//
// Created by andrey on 06.11.18.
//

#ifndef LAB4_OC_ACTION_H
#define LAB4_OC_ACTION_H

#define WINVER 0x0500
using namespace std;
#include <functional>
#include <map>
#include "Action.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>

class Action {
public:
    Action();

    std::function<void()> getAction(int num);

private:
    std::map<int, std::function<void()>> actionList;
    void createFile();
    void projection();
    void writing();
    HANDLE hFile;
    HANDLE hMapFile;
    LPVOID lpMapAddress;
  
};


#endif //LAB4_OC_ACTION_H
