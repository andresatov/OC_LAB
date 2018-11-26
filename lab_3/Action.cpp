//
// Created by andrey on 06.11.18.
//

#include <iostream>
#include "Action.h"

Action::Action() {
    actionList = {{1,  std::bind(&Action::outputDiskList, this)},
                  {2,  std::bind(&Action::outputInfoDisk, this)},
                  {3,  std::bind(&Action::createDir, this)},
                  {4,  std::bind(&Action::deleteDir, this)},
                  {5,  std::bind(&Action::createFile, this)},
                  {6,  std::bind(&Action::deleteFile, this)},
                  {7,  std::bind(&Action::copyFile, this)},
                  {8,  std::bind(&Action::moveFile, this)},
                  {9,  std::bind(&Action::getFileAtt, this)},
                  {10,  std::bind(&Action::getFileInd, this)},
                  {11, std::bind(&Action::setFileAtt, this)},
                  {12, std::bind(&Action::getFileTime, this)},
                  {13, std::bind(&Action::setFileTime, this)}
    };
}

std::function<void()> Action::getAction(int num) {
    return actionList[num];
}

void Action::outputDiskList() {

}

void Action::createDir() {

}

void Action::outputInfoDisk() {

}

void Action::deleteDir() {

}

void Action::createFile() {

}

void Action::deleteFile() {

}

void Action::copyFile() {

}

void Action::moveFile() {

}

void Action::getFileAtt() {

}

void Action::setFileAtt() {

}

void Action::getFileInd() {

}

void Action::getFileTime() {

}

void Action::setFileTime() {

}
