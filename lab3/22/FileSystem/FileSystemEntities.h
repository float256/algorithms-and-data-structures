//
// Created by user on 10/1/20.
//

#ifndef SIMPLEFILESYSTEM_FILESYSTEMENTITIES_H
#define SIMPLEFILESYSTEM_FILESYSTEMENTITIES_H

#include <string>


enum FileSystemElementType {
    FileSystemFile,
    FileSystemFolder
};

struct TextFileInfo {
    std::string Name;
    std::string Text;
};

struct FolderInfo {
    std::string Name;
};

#endif //SIMPLEFILESYSTEM_FILESYSTEMENTITIES_H
