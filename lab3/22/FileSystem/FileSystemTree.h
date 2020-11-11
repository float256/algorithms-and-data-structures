//
// Created by user on 10/1/20.
//

#ifndef SIMPLEFILESYSTEM_FILESYSTEMTREE_H
#define SIMPLEFILESYSTEM_FILESYSTEMTREE_H

#include <string>
#include <variant>
#include <stdexcept>
#include <iostream>
#include "FileSystemEntities.h"
#include "../DataStructures/Stack.h"

class FileSystemTree {
private:

    TreeNode* RootFolder = new TreeNode{
        .Data = FolderInfo {
            .Name = "/"
        },
        .ElementType = FileSystemFolder
    };
    TreeNode* CurrentFolder = RootFolder;
    Stack<std::string>* AbsolutePathToCurrentFolder= new Stack<std::string>();

    template<class T>
    bool IsItemInArray(T* item, T* arrayStartPtr, int arrayLength) {
        if (arrayLength < 0) {
            return false;
        }

        T* arrayEndPtr = arrayStartPtr + arrayLength;
        T* currPtr = arrayStartPtr;
        bool isFound = false;

        while ((currPtr != arrayEndPtr) && !isFound) {
            isFound = (currPtr == item);
            ++currPtr;
        }
        return isFound;
    }

    void PrintLevelDesignation(std::string levelDesignation, int currLevel = 1) {
        for (int i = 0; i < currLevel; ++i) {
            std::cout << levelDesignation;
        }
    }

    void PrintOneNode(TreeNode* currNode, TreeNode* skippedNodes, int skippedNodesLength,
                      std::string levelDesignation, int currLevel = 0, bool isPrintIndexInFolder = false,
                      int indexInFolder = 0) {
        if ((currNode != nullptr) && !IsItemInArray<TreeNode>(currNode, skippedNodes, skippedNodesLength)) {
            PrintLevelDesignation(levelDesignation, currLevel);
            if (isPrintIndexInFolder && (currNode != RootFolder)) {
                std::cout << "(" << indexInFolder << "): ";
            }

            std::variant<TextFileInfo, FolderInfo> currNodeInfo = currNode->Data;
            if (currNode->ElementType == FileSystemFolder) {
                TreeNode* currChildNode = currNode ->FirstChildNode;
                int childNodeIdx = 0;

                if (currNode == RootFolder) {
                    std::cout << std::get<FolderInfo>(currNodeInfo).Name << " (root folder)" << std::endl;
                } else if (currNode == CurrentFolder) {
                    std::cout << std::get<FolderInfo>(currNodeInfo).Name << " (current folder)" << std::endl;
                } else {
                    std::cout << std::get<FolderInfo>(currNodeInfo).Name << " (folder)" << std::endl;
                }

                while (currChildNode != nullptr) {
                    PrintOneNode(currChildNode, skippedNodes, skippedNodesLength, levelDesignation, currLevel + 1,
                                 isPrintIndexInFolder, childNodeIdx);
                    currChildNode = currChildNode ->NextNeighbourNode;
                    ++childNodeIdx;
                }
            } else {
                std::cout << std::get<TextFileInfo>(currNodeInfo).Name << " (file)\n";
            }
        }
    }

public:
    FileSystemTree() {
        AbsolutePathToCurrentFolder -> Push("");
    }

    ~FileSystemTree() {
        delete RootFolder;
        RootFolder = CurrentFolder = nullptr;
        delete AbsolutePathToCurrentFolder;
    }

    TreeNode* GetCurrentFolder() {
        return CurrentFolder;
    }

    TreeNode* GetRootFolder() {
        return RootFolder;
    }

    int GetNumberOfElementsInCurrentFolder() {
        TreeNode* currChild = CurrentFolder -> FirstChildNode;
        int numberOfElements = 0;

        while (currChild != nullptr) {
            ++numberOfElements;
            currChild = currChild -> NextNeighbourNode;
        }

        return numberOfElements;
    }

    void AddFolder(std::string name) {
        TreeNode* previousChildNode = CurrentFolder -> FirstChildNode;
        FolderInfo newFolderInfo = FolderInfo {
            .Name = name
        };
        CurrentFolder -> FirstChildNode = new TreeNode {
            .Data = newFolderInfo,
            .NextNeighbourNode = previousChildNode,
            .ParentNode = CurrentFolder,
            .ElementType = FileSystemFolder
        };
    }

    void AddTextFile(std::string name, std::string text) {
        TreeNode* previousChildNode = CurrentFolder -> FirstChildNode;
        TextFileInfo newTextFileInfo = TextFileInfo {
            .Name = name,
            .Text = text
        };
        CurrentFolder -> FirstChildNode = new TreeNode {
            .Data = newTextFileInfo,
            .NextNeighbourNode = previousChildNode,
            .ParentNode = CurrentFolder,
            .ElementType = FileSystemFile
        };
    }

    TreeNode* GetChildNode(int indexInFolder) {
        TreeNode* childNode = CurrentFolder -> FirstChildNode;
        if (indexInFolder > GetNumberOfElementsInCurrentFolder() - 1) {
            throw std::out_of_range("Out of range when trying to get child node");
        }
        for (int i = 0; i < indexInFolder; ++i) {
            childNode = childNode -> NextNeighbourNode;
        }
        return childNode;
    }

    void CheckoutToChildNode(int childNodeOrder) {
        TreeNode* childNode = GetChildNode(childNodeOrder);

        if (childNode -> ElementType != FileSystemFolder) {
            throw std::invalid_argument("Node with specified order is not a folder");
        }
        CurrentFolder = childNode;
        AbsolutePathToCurrentFolder -> Push(std::get<FolderInfo>(CurrentFolder -> Data).Name);
    }

    void CheckoutToParentNode() {
        if (CurrentFolder -> ParentNode != nullptr) {
            CurrentFolder = CurrentFolder -> ParentNode;
            AbsolutePathToCurrentFolder -> Pop();
        } else {
            throw std::out_of_range("Current folder hasn't parent folder");
        }
    }

    void AddExistingNode(TreeNode* node){
        TreeNode* previousChildNode = CurrentFolder -> FirstChildNode;
        node -> NextNeighbourNode = CurrentFolder -> NextNeighbourNode;
        CurrentFolder -> NextNeighbourNode = previousChildNode;
    }

    void PrintTree(TreeNode* startNode, TreeNode* skippedNodes= nullptr, int skippedNodesLength= 0,
                   std::string levelDesignation = "----", bool isPrintIndexInFolder = false) {
        PrintOneNode(startNode, skippedNodes, skippedNodesLength, levelDesignation, 0, isPrintIndexInFolder);
    }

    void PrintFolderContent(TreeNode* folder, TreeNode* skippedNodes = nullptr, int skippedNodesLength = 0,
                            bool isPrintIndexInFolder = false) {
        if (folder -> ElementType != FileSystemFolder) {
            throw std::invalid_argument("Node is not a folder");
        }

        TreeNode* currChildNode = folder -> FirstChildNode;
        int currChildNodeIdx = 0;
        while (currChildNode != nullptr) {
            if (isPrintIndexInFolder) {
                std::cout << "(" << currChildNodeIdx << ") ";
            }

            if (currChildNode -> ElementType == FileSystemFolder) {
                std::cout << std::get<FolderInfo>(currChildNode -> Data).Name << " (folder)" << std::endl;
            } else {
                std::cout << std::get<TextFileInfo>(currChildNode -> Data).Name << " (file)" << std::endl;
            }

            currChildNode = currChildNode -> NextNeighbourNode;
            ++currChildNodeIdx;
        }
    }

    void PrintPathToCurrentFolder() {
        if (AbsolutePathToCurrentFolder -> GetLength() == 0) {
            std::cout << std::get<FileSystemFolder>(RootFolder -> Data).Name;
        } else {
            std::string *startPathArrayPtr = AbsolutePathToCurrentFolder->ToArray();
            std::string *endArrayPtr = startPathArrayPtr + AbsolutePathToCurrentFolder->GetLength();

            std::string *currItemPtr = endArrayPtr - 1;
            while (currItemPtr >= startPathArrayPtr) {
                std::cout << (*currItemPtr) << "/";
                --currItemPtr;
            }
        }
    }

    void RemoveChildNode(int childNodeOrder) {
        TreeNode* currNode = GetChildNode(childNodeOrder);
        TreeNode* nextNode = currNode -> NextNeighbourNode;

        if (childNodeOrder == 0) {
            CurrentFolder -> FirstChildNode = nextNode;
        } else {
            GetChildNode(childNodeOrder - 1) -> NextNeighbourNode = nextNode;
        }
        currNode -> NextNeighbourNode = nullptr;
        delete currNode;
    }

    bool isExistNodeWithSameName(std::string name) {
        TreeNode* currChildNode = CurrentFolder -> FirstChildNode;
        bool isExist = false;

        while ((currChildNode != nullptr) && !isExist) {
            if (currChildNode -> ElementType == FileSystemFile) {
                isExist = (std::get<TextFileInfo>(currChildNode -> Data).Name == name);
            } else {
                isExist = (std::get<FolderInfo>(currChildNode -> Data).Name == name);
            }

            currChildNode = currChildNode -> NextNeighbourNode;
        }
        return isExist;
    }
};


#endif //SIMPLEFILESYSTEM_FILESYSTEMTREE_H
