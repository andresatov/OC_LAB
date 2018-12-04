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
    void systemInfo();
    void virtualMemoryStatus();
    void virtualPageStatus(DWORD address);
    void separateReserveCommit();
    void simultaneousReserveCommit();
    void writeData();
    void protectVirtualPage();
    void freeVirtualPage(DWORD address);
};


#endif //LAB4_OC_ACTION_H
