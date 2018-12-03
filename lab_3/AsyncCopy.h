//
// Created by andrey on 03.12.18.
//

#ifndef LAB3_OC_ASYNCCOPY_H
#define LAB3_OC_ASYNCCOPY_H
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void asyncCopyOfFile();
VOID WINAPI asyncRead(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv);
VOID WINAPI asyncWrite(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv);
#endif //LAB3_OC_ASYNCCOPY_H
