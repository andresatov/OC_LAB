//
// Created by andrey on 06.11.18.
//

#ifndef LAB3_OC_ACTION_H
#define LAB3_OC_ACTION_H

#include "AsyncCopy.h"
#include <functional>
#include <map>
#include <filesystem>

class Action {
public:
    Action();

    std::function<void()> getAction(int num);

private:
    std::map<int, std::function<void()>> actionList;

    void outputDiskList();

    void outputInfoDisk();

    void createDir();

    void deleteDir();

    void createFile();

    void deleteFile();

    void copyFile();

    void moveFile();

    void getFileAtt();

    void setFileAtt();

    void getFileTime();

    void setFileTime();
#ifdef WINAPI
    bool isDirectoryExists(const wchar_t *filename);
#else
    bool checkIsPath(std::filesystem::path path);
#endif
};


#endif //LAB3_OC_ACTION_H
