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
    int NumberOfElementsInCurrentFolder = 0;
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

    void PrintOneNode(TreeNode* currNode, TreeNode* skippedNodes, int skippedNodesLength,
                      std::string levelDesignation, int currLevel = 0, bool isPrintCurrentFolder = true,
                      bool isPrintRootFolder = false) {
        if ((currNode != nullptr) && !IsItemInArray<TreeNode>(currNode, skippedNodes, skippedNodesLength)) {
            for (int i = 0; i < currLevel; ++i) {
                std::cout << levelDesignation;
            }
            std::variant<TextFileInfo, FolderInfo> currNodeInfo = currNode->Data;
            if (currNode->ElementType == FileSystemFolder) {
                bool isPrintNodeData = ((currNode == RootFolder) && isPrintRootFolder) ||
                                       ((currNode == CurrentFolder) && isPrintCurrentFolder) ||
                                       ((currNode != RootFolder) && (currNode != CurrentFolder));
                if (isPrintNodeData) {
                    std::cout << std::get<FolderInfo>(currNodeInfo).Name << " (folder)\n";
                }
                TreeNode* currChildNode = currNode ->FirstChildNode;
                while (currChildNode != nullptr) {
                    PrintOneNode(currChildNode, skippedNodes, skippedNodesLength, levelDesignation, currLevel + 1);
                    currChildNode = currChildNode ->NextNeighbourNode;
                }
            } else {
                std::cout << std::get<TextFileInfo>(currNodeInfo).Name << " (file)\n";
            }
        }
    }


public:
    TreeNode* GetCurrentFolder() {
        return CurrentFolder;
    }

    TreeNode* GetRootFolder() {
        return RootFolder;
    }

    int GetNumberOfElementsInCurrentFolder() {
        return NumberOfElementsInCurrentFolder;
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
        ++NumberOfElementsInCurrentFolder;
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
        ++NumberOfElementsInCurrentFolder;
    }

    TreeNode* GetChildNode(int indexInFolder) {
        TreeNode* childNode = CurrentFolder -> FirstChildNode;
        if (indexInFolder > NumberOfElementsInCurrentFolder - 1) {
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
        AbsolutePathToCurrentFolder -> Push(std::get<FolderInfo>(CurrentFolder -> Data).Name);
        CurrentFolder = childNode;
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

    void PrintTree(TreeNode* skippedNodes=nullptr, int skippedNodesLength=0, std::string levelDesignation = "----",
                   bool isPrintRootFolder = false, bool isPrintCurrentFolder = false) {
        PrintOneNode(RootFolder, skippedNodes, skippedNodesLength, levelDesignation, 0, isPrintRootFolder, isPrintCurrentFolder);
    }

    void PrintFolderContent(TreeNode* skippedNodes= nullptr, int skippedNodesLength=0, std::string levelDesignation = "----",
                            bool isPrintRootFolder = false, bool isPrintCurrentFolder = false) {
        PrintOneNode(CurrentFolder, skippedNodes, skippedNodesLength, levelDesignation, 0, isPrintRootFolder, isPrintCurrentFolder);
    }
};


#endif //SIMPLEFILESYSTEM_FILESYSTEMTREE_H
