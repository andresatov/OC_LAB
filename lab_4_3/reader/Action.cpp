//
// Created by andrey on 06.11.18.
//

#include <iostream>
#include "Action.h"

Action::Action() {
    actionList = {{1,  std::bind(&Action::open, this)},
                  {2,  std::bind(&Action::projection, this)},
                  {3,  std::bind(&Action::read, this)}
    };
}

std::function<void()> Action::getAction(int num) {
    return actionList[num];
    
}

void Action::open() {
    hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "myMappedFile");
    if (hMapFile != INVALID_HANDLE_VALUE)
        cout << "Проекция открыта\n";
    
}

void Action::projection() {
    lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (lpMapAddress == 0){
        cerr << "Не удаётся открыть проекцию файла\n";
        system("pause");
        exit(1);
    }
}

void Action::read() {
    char answer;
    char data[1024];
    memcpy(data, (char*)lpMapAddress, 1024);
    cout << "Данные по адресу (0x"<< lpMapAddress <<"): " << data << "\n";

    cout << "Закрыть проекцию? (y/n) ";
    cin >> answer;
    if (answer == 'y') {
        UnmapViewOfFile(lpMapAddress);
    }
    CloseHandle(hMapFile);
}
