//
// Created by andrey on 06.11.18.
//

#include <iostream>
#include "Action.h"
#include <iostream>
#include <stdio.h>

#ifdef _WIN32
#include<windows.h> // windows api
#include <conio.h>
#endif

#include <algorithm>
//#define WINAPI

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
    int n;
    char driveLetter;
    DWORD dr = GetLogicalDrives();

    std::cout << "GetLogicalDrives():" << std::endl;

    for (int i = 0; i < 26; i++) {
        n = ((dr >> i) & 0x1);
        if (n == 1) {
            driveLetter = char(65 + i);
            std::cout << driveLetter << std::endl;
        }
    }
    std::cout << std::endl << "GetLogicalDriveStrings():" << std::endl;

    wchar_t drives[256];
    wchar_t *drive;
    DWORD sizebuf = 256;
    GetLogicalDriveStrings(sizebuf, drives);
    drive = drives;
    while (*drive) {
        wprintf(L"%s\n", drive);
        drive = drive + wcslen(drive) + 1;
    }
#else
#endif

}

void Action::createDir() {

#ifdef WINAPI
    wchar_t directoryName[250];
	char directoryCharName[250];
	std::cout << "Entry path ";
	std::cin >> directoryCharName;
    mbstowcs(directoryName, directoryCharName, 250);
    if (CreateDirectory(directoryName, NULL)) {
        std::cout << "directory create" << std::endl;
    } else {
        std::cout << "error create directory" << std::endl;
    }
}
#else
    std::filesystem::path path;
    std::cout << "Entry path new Dir";
    std::cin >> path;
    if (std::filesystem::create_directory(path)) {
        std::cout << "File create" << std::endl;
    } else {
        std::cout << "File not create" << std::endl;
    }
#endif
}

void Action::outputInfoDisk() {

#ifdef WINAPI
    char driveLetter[100];
	wchar_t driveLetterWchar[100];
	cout << "Entry name: ";
	cin >> driveLetter;
	driveLetter[1] = ':';
	driveLetter[2] = '\\';
	driveLetter[3] = 0;
	mbstowcs(driveLetterWchar, driveLetter, 2);
	driveLetterWchar[2] = 0;

    switch (auto infoDisk = GetDriveType(driveLetterWchar); infoDisk) {
        case DRIVE_UNKNOWN :
            std::cout << "Disk " << infoDisk << "is an UNKNOWN " << std::endl;
            break;
        case DRIVE_NO_ROOT_DIR:
            std::cout << "Disk " << infoDisk << " missing " << std::endl;
            return;
        case DRIVE_REMOVABLE:
            std::cout << "Disk " << infoDisk << " is an removable" << std::endl;
            break;
        case DRIVE_FIXED:
            std::cout << "Disk " << infoDisk << "is an fixed" << std::endl;
            break;
        case DRIVE_REMOTE:
            std::cout << "Disk " << infoDisk << "is an remote" << std::endl;
            break;
        case DRIVE_CDROM:
            std::cout << "Disk " << infoDisk << "is an CDROM" << std::endl;
            break;
        case DRIVE_RAMDISK:
            std::cout << "Disk " << infoDisk << "is an RAM disk" << std::endl;
            break;
    }

  char volumeNameBuffer[100];
	volumeNameBuffer[0] = 0;
	char fileSystemNameBuffer[100];
	fileSystemNameBuffer[0] = 0;
	DWORD maxComponentLength = 0, systemFlags = 0; //fs - системные флаги
	unsigned long drive_sn = 0;
	GetVolumeInformationA(driveLetter, volumeNameBuffer, 100, &drive_sn, &maxComponentLength, &systemFlags, fileSystemNameBuffer, 100); //ANSI
	cout << "Имя диска: " << volumeNameBuffer << endl <<
		"Серийный номер: " << drive_sn << endl <<
		"Тип файловой системы: " << fileSystemNameBuffer << endl <<
		"Системные флаги: " <<endl;

	string TSV = "The specified volume";
	string TSVS = TSV +" supports";

	if (systemFlags & FILE_CASE_PRESERVED_NAMES)
		std::cout<<TSVS+" preserved case of file names when it places a name on disk.\n";
	if (systemFlags & FILE_CASE_SENSITIVE_SEARCH)
		std::cout<< TSVS + " case-sensitive file names.\n";
	if (systemFlags & FILE_FILE_COMPRESSION)
		std::cout << TSVS + " file-based compression.\n";
	if (systemFlags & FILE_NAMED_STREAMS)
		std::cout << TSVS + " named streams.\n";
	if (systemFlags & FILE_PERSISTENT_ACLS)
		std::cout << TSV + " preserves and enforces access control lists (ACL). For example, the NTFS file system preserves and enforces ACLs, and the FAT file system does not.\n";
	if (systemFlags & FILE_READ_ONLY_VOLUME)
		std::cout << TSV + " is read-only.\n";
	if (systemFlags & FILE_SEQUENTIAL_WRITE_ONCE)
		std::cout << TSVS + " a single sequential write.\n";
	if (systemFlags & FILE_SUPPORTS_ENCRYPTION)
		std::cout << TSVS + " the Encrypted File System (EFS).\n";
	if (systemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
		std::cout << TSVS + " extended attributes.\n";
	if (systemFlags & FILE_SUPPORTS_HARD_LINKS)
		std::cout << TSVS + " hard links. \n";
	if (systemFlags & FILE_SUPPORTS_OBJECT_IDS)
		std::cout << TSVS + " object identifiers.\n";
	if (systemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
		std::cout << TSVS + " open by FileID.\n";
	if (systemFlags & FILE_SUPPORTS_REPARSE_POINTS)
		std::cout << TSVS + " reparse points.\n";
	if (systemFlags & FILE_SUPPORTS_SPARSE_FILES)
		std::cout << TSVS + " sparse files.\n";
	if (systemFlags & FILE_SUPPORTS_TRANSACTIONS)
		std::cout << TSVS + " transactions.\n";
	if (systemFlags & FILE_SUPPORTS_USN_JOURNAL)
		std::cout << TSVS + " update sequence number (USN) journals.\n";
	if (systemFlags & FILE_UNICODE_ON_DISK)
		std::cout << TSVS + " Unicode in file names as they appear on disk.\n";
	if (systemFlags & FILE_VOLUME_IS_COMPRESSED)
		std::cout << TSV + " is a compressed volume, for example, a DoubleSpace volume.\n";
	if (systemFlags & FILE_VOLUME_QUOTAS)
		std::cout << TSVS + " disk quotas.\n";

	DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
	GetDiskFreeSpaceA(driveLetter, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters);
	unsigned __int64 free = freeClusters * sectorsPerCluster / 1024 * bytesPerSector / 1024;
	unsigned __int64 total = totalClusters* sectorsPerCluster / 1024 * bytesPerSector / 1024;
    std::cout << "Дисковое пространство (свободное/всего): " << free << " / " << total << " MiB\n";

    }
#else
    std::string disk;
    std::cout << "Output disk information" << std::endl;
    std::cout << "Entry disk: " << std::endl;
    std::cin >> disk;
    if (!checkIsPath(disk)) {
        return;
    }
    std::filesystem::space_info memory_disk = std::filesystem::space(disk);
    std::cout << ".        Capacity       Free      Available" << std::endl
              << disk << "   " << memory_disk.capacity << "   "
              << memory_disk.free << "   " << memory_disk.available << std::endl;
#endif

}

void Action::deleteDir() {

#ifdef WINAPI
    wchar_t directoryName[250];
	char directoryCharName[250];
	std::cout << "Entry path ";
	std::cin >> directoryCharName;
    mbstowcs(directoryName, directoryCharName, 250);
    if (!RemoveDirectory(directoryName)){
        std::cout<<"Dir not remove!";
    };
#else
    std::cout << "Entry path delete dir";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    if (!std::filesystem::remove_all(path)){
        std::cout<<"Dir not remove!";
    }
#endif
    std::cout<<"Dir remove!";
}

void Action::createFile() {

#ifdef WINAPI
    wchar_t directoryName[250];
	char directoryCharName[250];
	std::cout << "Entry path ";
	std::cin >> directoryCharName;
    mbstowcs(directoryName, directoryCharName, 250);
    DWORD dwCounter, dwTemp;
    HANDLE hFile = CreateFile(directoryName, GENERIC_READ, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        std::cout << "not create File" << std::endl;
        return;
    }
    std::cout << "Create File" << std::endl;
    CloseHandle(hFile)
#else
    std::cout << "Entry path new file";
    std::string path;
    std::cin >> path;
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


#ifdef WINAPI
    wchar_t directoryName[250];
	char directoryCharName[250];
	std::cout << "Entry path ";
	std::cin >> directoryCharName;
    mbstowcs(directoryName, directoryCharName, 250);
    if (!RemoveDirectory(directoryName)){
        std::cout<<"File not remove!";
    }
#else
    std::cout << "Entry path delete file";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }

    if (!std::filesystem::remove(path)){
        std::cout<<"File not remove!";
    }
#endif
    std::cout<<"File remove!"<< std::endl;
}


void Action::copyFile() {


#ifdef WINAPI
    wchar_t source[250], destination[250];
    std::cout << "Entry path ";
    std::cin >> sourceChar;
    std::cout << "Entry copy path";
    std::cin >> destinationChar;
    char sourceChar[250], destinationChar[250];
    mbstowcs(source, sourceChar, 250);
    mbstowcs(destination, destinationChar, 250);
    if (CopyFile(source, destination, false) != 0) {
        std::cout << "File copy" << std::endl;
    } else {
        std::cout << "Eror copy" << std::endl;
    }
#else
    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    std::cout << "Entry copy path";
    std::string copy_path;
    std::cin >> copy_path;

    try {
        std::filesystem::copy_file(path, copy_path);
        std::cout << "File copy" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << "Could not copy : " << e.what() << std::endl;

    }
#endif
}

void Action::moveFile() {

#ifdef WINAPI
    wchar_t source[250], destination[250];
    std::cout << "Entry path ";
    std::cin >> sourceChar;
    std::cout << "Entry move path";
    std::cin >> destinationChar;
    char sourceChar[250], destinationChar[250];
    mbstowcs(source, path.c_str(), 250);
    mbstowcs(destination, destinationChar.c_str(), 250);
    if (MoveFile(source, move_path) != 0) {
        std::cout << "File move" << std::endl;
    } else {
        std::cout << "Could not move " << std::endl;
    }
#else
    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    std::cout << "Entry move path";
    std::string move_path;
    std::cin >> move_path;

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
    wchar_t fileName[250];
	char fileCharName[250];
	std::cout << "Entry path ";
	std::cin >> fileCharName;
    mbstowcs(fileName, fileCharName, 250);
    DWORD fileAttributes;
    fileAttributes = GetFileAttributes(fileName);
    tempStringStream << "0x";
    tempStringStream << hex << fileAttributes << "\n";
    std::cout << "Аттрибуты: " << tempStringStream.str() << endl;
    if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE)
        std::cout
                << "FILE_ATTRIBUTE_ARCHIVE:\nA file or directory that is an archive file or directory. Applications typically use this attribute to mark files for backup or removal.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_COMPRESSED)
        std::cout
                << "FILE_ATTRIBUTE_COMPRESSED:\nA file or directory that is compressed. For a file, all of the data in the file is compressed. For a directory, compression is the default for newly created files and subdirectories.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_DEVICE)
        std::cout << "FILE_ATTRIBUTE_DEVICE:\nThis value is reserved for system use.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        std::cout << "FILE_ATTRIBUTE_DIRECTORY:\nThe handle that identifies a directory.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
        std::cout
                << "FILE_ATTRIBUTE_ENCRYPTED:\nA file or directory that is encrypted. For a file, all data streams in the file are encrypted. For a directory, encryption is the default for newly created files and subdirectories.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_HIDDEN)
        std::cout
                << "FILE_ATTRIBUTE_HIDDEN:\nThe file or directory is hidden. It is not included in an ordinary directory listing.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
        std::cout << "FILE_ATTRIBUTE_INTEGRITY_STREAM:\nThe directory or user data stream is configured with integrity (only supported on ReFS volumes). It is not included in an ordinary directory listing. The integrity setting persists \
             with the file if it's renamed. If a file is copied the destination file will have integrity set if either the source file or destination directory have integrity set.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_NORMAL)
        std::cout
                << "FILE_ATTRIBUTE_NORMAL:\nA file that does not have other attributes set. This attribute is valid only when used alone.\n";
    if (fileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
        std::cout
                << "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED:\nThe file or directory is not to be indexed by the content indexing service.\n";
    if (fileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
        std::cout << "FILE_ATTRIBUTE_NO_SCRUB_DATA:\nThe user data stream not to be read by the background data integrity scanner (AKA scrubber). When set on a directory it only provides inheritance. This flag is only supported on\
                Storage Spaces and ReFS volumes. It is not included in an ordinary directory listing.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_OFFLINE)
        std::cout << "FILE_ATTRIBUTE_OFFLINE:\nThe data of a file is not available immediately. This attribute indicates that the file data is physically moved to offline storage. This attribute is used by Remote Storage,\
                 which is the hierarchical storage management software. Applications should not arbitrarily change this attribute.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_READONLY)
        std::cout
                << "FILE_ATTRIBUTE_READONLY:\nA file that is read-only. Applications can read the file, but cannot write to it or delete it. This attribute is not honored on directories.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
        std::cout
                << "FILE_ATTRIBUTE_REPARSE_POINT:\nA file or directory that has an associated reparse point, or a file that is a symbolic link.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
        std::cout << "FILE_ATTRIBUTE_SPARSE_FILE:\nA file that is a sparse file.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_SYSTEM)
        std::cout
                << "FILE_ATTRIBUTE_SYSTEM:\nA file or directory that the operating system uses a part of, or uses exclusively.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_TEMPORARY)
        std::cout << "FILE_ATTRIBUTE_TEMPORARY:\nA file that is being used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application\
                 deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed.\n--\n";
    if (fileAttributes & FILE_ATTRIBUTE_VIRTUAL)
        std::cout << "FILE_ATTRIBUTE_VIRTUAL:\nThis value is reserved for system use.\n--\n";

#else
#endif
}

void Action::setFileAtt() {
#ifdef WINAPI
    wchar_t fileName[250];
	char fileCharName[250];
	std::cout << "Entry path ";
	std::cin >> fileCharName;
    mbstowcs(fileName, fileCharName, 250);
    DWORD attrs = GetFileAttributesA(fileName);

    char answer;

    std::cout << "Сделать архивным? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_ARCHIVE;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_ARCHIVE;
    }
    std::cout << "Сделать невидимым? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_HIDDEN;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_HIDDEN;
    }
    std::cout << "Сделать обычным? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_NORMAL;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_NORMAL;
    }
    std::cout << "Индексировать содержание? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    }
    std::cout << "Доступен без сети? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_OFFLINE;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_OFFLINE;
    }
    std::cout << "Сделать доступным только для чтения? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_READONLY;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_READONLY;
    }
    std::cout << "Сделать системным? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_SYSTEM;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_SYSTEM;
    }
    std::cout << "Сделать временным? (y/n):";
    std::cin >> answer;
    if (answer == 'y'){
        attrs |= FILE_ATTRIBUTE_TEMPORARY;
    }
    else{
        attrs &= ~FILE_ATTRIBUTE_TEMPORARY;
    }
    if (SetFileAttributesA(fileName, attrs)){
        std::cout << "Аттрибуты успешно установленны!\n";
    }
    else{
        std::cout << "Произошла ошибка, аттрибуты не были установлены!\n";
    }
#else
#endif
}

void Action::getFileInd() {
#ifdef WINAPI

    char fileCharName[250];
	std::cout << "Введите имя файла (латинскими буквами, без пробелов): ";
	std::cin >> fileCharName;
    mbstowcs(fileName, fileCharName, 250);
    FILE* pfile = fopen(fileCharName, "r");
	HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(pfile));
	if (hFile == NULL){
		std::cout << "Невозможно получить обработчик файла!\n";
		return;
    }
    BY_HANDLE_FILE_INFORMATION fileinfo;
	if (GetFileInformationByHandle(hFile, &fileinfo)){
		std::cout << "\nСерийный номер тома: " << fileinfo.dwVolumeSerialNumber << endl
			<< "Количество ссылок: " << fileinfo.nNumberOfLinks << endl;
}
#else
#endif
}

void Action::getFileTime() {

#ifdef WINAPI
    wchar_t fileName[250];
	char fileCharName[250];
	std::cout << "Entry path ";
	std::cin >> fileCharName;
    mbstowcs(fileName, fileCharName, 250);
    FILE *pfile = fopen(fileCharName, "r");
    HANDLE hFile = (HANDLE) _get_osfhandle(_fileno(static_cast<wchar_t> (fileName)));

    if (hFile == NULL) {
        std::cout << "error\n";
        return;
    }
    FILETIME fileCreatedTime;
    SYSTEMTIME fileCreatedSystemTime;
    wchar_t createdLocalDate[255];
    wchar_t createdLocalTime[255];
    FILETIME fileAccessedTime;
    SYSTEMTIME fileAccessedSystemTime;
    wchar_t accessedLocalDate[255];
    wchar_t accessedLocalTime[255];
    FILETIME fileWritedTime;
    SYSTEMTIME fileWritedSystemTime;
    wchar_t writedLocalDate[255];
    wchar_t writedLocalTime[255];

    if (GetFileTime(hFile, &fileCreatedTime, &fileAccessedTime, &fileWritedTime) != 0) {
        FileTimeToLocalFileTime(&fileCreatedTime, &fileCreatedTime);
        FileTimeToLocalFileTime(&fileAccessedTime, &fileAccessedTime);
        FileTimeToLocalFileTime(&fileWritedTime, &fileWritedTime);

        FileTimeToSystemTime(&fileCreatedTime, &fileCreatedSystemTime);
        FileTimeToSystemTime(&fileAccessedTime, &fileAccessedSystemTime);
        FileTimeToSystemTime(&fileWritedTime, &fileWritedSystemTime);

        GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fileCreatedSystemTime, NULL, createdLocalDate, 255);
        GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fileAccessedSystemTime, NULL, accessedLocalDate, 255);
        GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fileWritedSystemTime, NULL, writedLocalDate, 255);

        GetTimeFormat(LOCALE_USER_DEFAULT, 0, &fileCreatedSystemTime, NULL, createdLocalTime, 255);
        GetTimeFormat(LOCALE_USER_DEFAULT, 0, &fileAccessedSystemTime, NULL, accessedLocalTime, 255);
        GetTimeFormat(LOCALE_USER_DEFAULT, 0, &fileWritedSystemTime, NULL, writedLocalTime, 255);

        std::cout << "Время создания: " << fileCreatedSystemTime.wDay << "." << fileCreatedSystemTime.wMonth << "."
             << fileCreatedSystemTime.wYear << " " << fileCreatedSystemTime.wHour << ":"
             << fileCreatedSystemTime.wMinute << "\n";
        std::cout << "Последнее обращение: " << fileAccessedSystemTime.wDay << "." << fileAccessedSystemTime.wMonth << "."
             << fileAccessedSystemTime.wYear << " " << fileAccessedSystemTime.wHour << ":"
             << fileAccessedSystemTime.wMinute << "\n";
        std::cout << "Последнее изменение: " << fileWritedSystemTime.wDay << "." << fileWritedSystemTime.wMonth << "."
             << fileWritedSystemTime.wYear << " " << fileWritedSystemTime.wHour << ":" << fileWritedSystemTime.wMinute
             << "\n";
#else

    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    auto ftime = std::filesystem::last_write_time(path);
    std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
    std::cout << "File write time is " << std::asctime(std::localtime(&cftime)) << std::endl;

#endif
}

void Action::setFileTime() {
#ifdef WINAPI
    wchar_t filename[250];
    char fileCharName[250];
    std::cout << "Entry path ";
    std::cin >> fileCharName;
    mbstowcs(filename, fileCharName, 250);
    HANDLE hFile = CreateFile(filename, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);

    FILETIME fileTime;
    SYSTEMTIME systemTimeNow;
    GetSystemTime(&systemTimeNow);
    SystemTimeToFileTime(&systemTimeNow, &fileTime);

    if (!SetFileTime(hFile, &fileTime, NULL, NULL))
        std::cout << "error\n";
}
CloseHandle(hFile);
#else
    std::cout << "Entry path ";
    std::string path;
    std::cin >> path;
    if (!checkIsPath(path)) {
        return;
    }
    std::filesystem::last_write_time(path, std::chrono::system_clock::time_point(std::chrono::system_clock::now()));
#endif
}

#ifdef WINAPI

bool Action::checkIsPath(std::string filename) {
    DWORD dwFileAttributes = GetFileAttributes(static_cast<wchar_t>(filename.c_str()));
    if (dwFileAttributes == 0xFFFFFFFF){
        std::cout << "Can't find file!" << filename << std::endl;
        return false;}
    return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

#else

bool Action::checkIsPath(std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
        std::cout << "Can't find file!" << path << std::endl;
        return false;
    }
    return true;
#endif
}