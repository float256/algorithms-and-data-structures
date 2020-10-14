//
// Created by user on 10/1/20.
//

#ifndef SIMPLEFILESYSTEM_FILESYSTEMTREE_H
#define SIMPLEFILESYSTEM_FILESYSTEMTREE_H

#include <string>
#include <variant>
#include "FileSystemEntities.h"

class FileSystemTree {
private:
    struct TreeNode {
        std::variant<TextFileInfo, FolderInfo> Data;
        TreeNode* NextNeighbourNode = nullptr;
        TreeNode* FirstChildNode = nullptr;
        TreeNode* ParentNode = nullptr;
        FileSystemElementType ElementType;
    };
    TreeNode* RootNode = new TreeNode{
        .Data = FolderInfo {
            .Name = "/"
        }
    };
    TreeNode* CurrentFolder = RootNode;
    int Count = 0;

public:
    void AddFolder(std::string name) {
        TreeNode* previousChildNode = CurrentFolder -> FirstChildNode;
        FolderInfo newFolderInfo = FolderInfo{
            .Name = name
        };
        CurrentFolder -> FirstChildNode = new TreeNode{
            .Data = newFolderInfo,
            .NextNeighbourNode = previousChildNode,
            .ParentNode = CurrentFolder,
        };
    }

    void AddExistingNode(TreeNode* node){
        TreeNode* previousChildNode = CurrentFolder -> FirstChildNode;
        node -> NextNeighbourNode = CurrentFolder -> NextNeighbourNode;
        CurrentFolder -> NextNeighbourNode = previousChildNode;
    }
};


#endif //SIMPLEFILESYSTEM_FILESYSTEMTREE_H
