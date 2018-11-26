//
// Created by andrey on 06.11.18.
//

#include <iostream>
#include "Action.h"
#include <iostream>
#include <stdio.h>

#ifdef _WIN32
#include<windows.h>
#include <conio.h>
#endif

#include <algorithm>
//#define WINAPI
#include <filesystem>
#include <iomanip>
#include <fstream>
#include "view.h"

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
                  {10, std::bind(&Action::getFileInd, this)},
                  {11, std::bind(&Action::setFileAtt, this)},
                  {12, std::bind(&Action::getFileTime, this)},
                  {13, std::bind(&Action::setFileTime, this)}
    };
}

std::function<void()> Action::getAction(int num) {
    return actionList[num];
}

void Action::outputDiskList() {
#ifdef WINAPI
    char buf[26];
    GetLogicalDriveStringsA(sizeof(buf),buf);
    char *DRF [] = {"Unknown" , "Invalid path",
                    "Removable", "Fixed" , "Network drive","CD-ROM", "RAM disk"};
    for(char *s=buf; *s; s+=strlen(s)+1){
        cout<<s<<"  "<<DRF[GetDriveTypeA(s)]<<endl;
    }
#else

#endif

}

void Action::createDir() {

    std::filesystem::path path;
    std::cout << "Entry path new Dir";
    std::cin >> path;
#ifdef WINAPI
    if (CreateDirectory("c:\\new",NULL)){
        std::cout << "directory create" << std::endl;}
    else{
        std::cout << "error create directory" << std::endl;}
}
#else
    if (std::filesystem::create_directory(path)) {
        std::cout << "File create" << std::endl;
    } else {
        std::cout << "File not create" << std::endl;
    }
#endif
}

void Action::outputInfoDisk() {

    std::string disk;
    std::cout << "output disk information" << std::endl;
    std::cout << "Entry disk: " << std::endl;
    std::cin >> disk;
    if (!checkIsPath(disk)) {
        return;
    }
#ifdef WINAPI
    switch(auto infoDisk=GetDriveType(disk.c_str());infoDisk){
        case DRIVE_UNKNOWN :
            std::cout<<"Disk "<<infoDisk<<"is an UNKNOWN "<<std::endl;
            break;
        case DRIVE_NO_ROOT_DIR:
            std::cout<<"Disk "<<infoDisk<<" missing "<<std::endl;
            return;
            break;
        case DRIVE_REMOVABLE:
            std::cout<<"Disk "<<infoDisk<<" is an removable"<<std::endl;
            break;
        case DRIVE_FIXED:
             std::cout<<"Disk "<<infoDisk<<"is an fixed"<<std::endl;
            break;
        case DRIVE_REMOTE:
            std::cout<<"Disk "<<infoDisk<<"is an remote"<<std::endl;
            break;
        case DRIVE_CDROM:
            std::cout<<"Disk "<<infoDisk<<"is an CDROM"<<std::endl;
            break;
        case DRIVE_RAMDISK:
            std::cout<<"Disk "<<infoDisk<<"is an RAM disk"<<std::endl;
            break;
    }
    if (char NameBuffer[MAX_PATH];char SysNameBuffer[MAX_PATH];DWORD VSNumber;DWORD MCLength;DWORD FileSF;
    GetVolumeInformation((disk+"\\").c_str(),NameBuffer, sizeof(NameBuffer),
        &VSNumber,&MCLength,&FileSF,SysNameBuffer,sizeof(SysNameBuffer)))
    {
        cout <<  NameBuffer << endl;
        cout <<  SysNameBuffer << endl;
        cout <<  VSNumber << endl;
    }

    if( DWORD FreeBytesAvailable;
    DWORD TotalNumberOfBytes;
    DWORD TotalNumberOfFreeBytes;
    GetDiskFreeSpaceEx((disk+"\\").c_str(),
    (PULARGE_INTEGER)&FreeBytesAvailable,
    (PULARGE_INTEGER)&TotalNumberOfBytes,
    (PULARGE_INTEGER)&TotalNumberOfFreeBytes
    ))
    {
        cout << "	Total Number Of Free Bytes = " << (unsigned long)TotalNumberOfFreeBytes
         << "( " <<  double(unsigned long(TotalNumberOfFreeBytes))/1024/1000
         << " Mb )" << endl;
        cout << "	Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes
         << "( " << double(unsigned long(TotalNumberOfBytes))/1024/1000
         << " Mb )" << endl;
    }
    else	cout << "	Not Present (GetDiskFreeSpace)" << endl;

    if( DWORD FreeBytesAvailable;
    DWORD TotalNumberOfBytes;
    DWORD TotalNumberOfFreeBytes;
    GetDiskFreeSpaceEx((disk+"\\").c_str(),
    (PULARGE_INTEGER)&FreeBytesAvailable,
    (PULARGE_INTEGER)&TotalNumberOfBytes,
    (PULARGE_INTEGER)&TotalNumberOfFreeBytes
    ))
    {
        cout << "	Total Number Of Free Bytes = " << (unsigned long)TotalNumberOfFreeBytes
         << "( " <<  double(unsigned long(TotalNumberOfFreeBytes))/1024/1000
         << " Mb )" << endl;
        cout << "	Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes
         << "( " << double(unsigned long(TotalNumberOfBytes))/1024/1000
         << " Mb )" << endl;
    }
    else	cout << "	Not Present (GetDiskFreeSpace)" << endl;
#else

    std::filesystem::space_info memory_disk = std::filesystem::space(disk);
    std::cout << ".        Capacity       Free      Available" << std::endl
              << disk << "   " << memory_disk.capacity << "   "
              << memory_disk.free << "   " << memory_disk.available << std::endl;
#endif

}

void Action::deleteDir() {
    std::cout << "Entry path delete dir";
    std::string path;
    std::cin >> path;
    std::ofstream file(path);
    if (!checkIsPath(path)) {
        return;
    }
#ifdef WINAPI
    RemoveDirectory(path.c_str());
#else
    std::filesystem::remove_all(path);
#endif
}

void Action::createFile() {
    std::cout << "Entry path new file";
    std::string path;
    std::cin >> path;
#ifdef WINAPI
    DWORD dwCounter, dwTemp;
  HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(INVALID_HANDLE_VALUE == hFile) {
      std::cout<<"not create File"<<std::endl;
      return 0;
  }
   std::cout<<"Create File"<<std::endl;
  CloseHandle(hFile)
#else
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cout << "not create File" << std::endl;
        return;
    }
    std::cout << "Create File" << std::endl;
    file.close();

#endif
}

void Action::deleteFile() {

    std::cout << "Entry path delete file";
    std::string path;
    std::cin >> path;
    std::ofstream file(path);
    if (!checkIsPath(path)) {
        return;
    }
#ifdef WINAPI

#else
    std::cout << "Remove File" << std::endl;
    std::filesystem::remove(path);
#endif
}


void Action::copyFile() {
    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    std::cout << "Entry copy path";
    std::string copy_path;
    std::cin >> copy_path;

#ifdef WINAPI
    wchar_t source[250], destination[250];
    char sourceChar[250], destinationChar[250];
    mbstowcs(source, sourceChar, 250);
    mbstowcs(destination, destinationChar, 250);
    if (CopyFile(source, destination, false) != 0){
       std::cout<<"File copy"<<std::endl;}
    else{
       std::cout<<"Eror copy"<<std::endl;}
#else
    try {
        std::filesystem::copy_file(path, copy_path);
        std::cout << "File copy" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << "Could not copy : " << e.what() << std::endl;

    }
#endif
}

void Action::moveFile() {
    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    std::cout << "Entry move path";
    std::string move_path;
    std::cin >> move_path;

#ifdef WINAPI
    wchar_t source[250], destination[250];
    mbstowcs(source,path.c_str(), 250);
    mbstowcs(destination, destinationChar.c_str(), 250);
    if (MoveFile(source,  move_path) != 0){
         std::cout<<"File move"<<std::endl;}
    else{
     std::cout << "Could not move " << std::endl;}
#else
    try {
        std::filesystem::rename(path, move_path);
        std::cout << "File move" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << "Could not move : " << e.what() << std::endl;

    }
#endif
}

void Action::getFileAtt() {
#ifdef WINAPI

#else
#endif
}

void Action::setFileAtt() {
#ifdef WINAPI

#else
#endif
}

void Action::getFileInd() {
#ifdef WINAPI

#else
#endif
}

void Action::getFileTime() {

    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
#ifdef WINAPI

#else
    auto ftime = std::filesystem::last_write_time(path);
    std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
    std::cout << "File write time is " << std::asctime(std::localtime(&cftime)) << std::endl;

#endif
}

void Action::setFileTime() {
    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
#ifdef WINAPI

#else
    std::filesystem::last_write_time(path,std::chrono::system_clock::time_point(std::chrono::system_clock::now()));
#endif
}
