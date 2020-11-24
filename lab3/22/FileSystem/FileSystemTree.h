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
#include "../DataStructures/Queue.h"

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

    void PrintLevelDesignation(std::string levelDesignation, int currLevel = 1) {
        for (int i = 0; i < currLevel; ++i) {
            std::cout << levelDesignation;
        }
    }

    void PrintOneNode(TreeNode* currNode, std::string levelDesignation, int currLevel = 0,
                      bool isPrintIndexInFolder = false, int indexInFolder = 0, bool isPrintFileContent = false) {
        if (currNode != nullptr) {
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
                    PrintOneNode(currChildNode, levelDesignation, currLevel + 1,
                                 isPrintIndexInFolder, childNodeIdx);
                    currChildNode = currChildNode ->NextNeighbourNode;
                    ++childNodeIdx;
                }
            } else {
                std::cout << std::get<TextFileInfo>(currNodeInfo).Name << " (file)" << std::endl;
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
        TreeNode* newNode = new TreeNode {
            .Data = FolderInfo{
                .Name = name
            },
            .ParentNode = CurrentFolder,
            .ElementType = FileSystemFolder
        };
        if (CurrentFolder -> FirstChildNode != nullptr) {
            TreeNode* lastNode = CurrentFolder -> FirstChildNode;
            while (lastNode -> NextNeighbourNode != nullptr) {
                lastNode = lastNode -> NextNeighbourNode;
            }
            lastNode -> NextNeighbourNode = newNode;
        }
        else
        {
            CurrentFolder -> FirstChildNode = newNode;
        }
    }

    void AddTextFile(std::string name, std::string text) {
        TreeNode* newNode = new TreeNode {
            .Data = TextFileInfo {
                .Name = name,
                .Text = text
            },
            .ParentNode = CurrentFolder,
            .ElementType = FileSystemFile
        };

        if (CurrentFolder -> FirstChildNode != nullptr) {
            TreeNode* lastNode = CurrentFolder -> FirstChildNode;
            while (lastNode -> NextNeighbourNode != nullptr) {
                lastNode = lastNode -> NextNeighbourNode;
            }
            lastNode -> NextNeighbourNode = newNode;
        }
        else {
            CurrentFolder -> FirstChildNode = newNode;
        }
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

    void MoveChildNode(TreeNode* newParentNode, int childNodeOrder){
        TreeNode* childNode = GetChildNode(childNodeOrder);

        RemoveNodeFromTree(childNodeOrder, false);
        if (newParentNode -> FirstChildNode != nullptr) {
            TreeNode* lastNodeInNewParentNode = newParentNode -> FirstChildNode;
            while(lastNodeInNewParentNode -> NextNeighbourNode != nullptr) {
                lastNodeInNewParentNode = lastNodeInNewParentNode -> NextNeighbourNode;
            }

            childNode -> NextNeighbourNode = nullptr;
            childNode -> ParentNode = newParentNode;
            lastNodeInNewParentNode -> NextNeighbourNode = childNode;
        }
        else {
            childNode -> NextNeighbourNode = nullptr;
            newParentNode -> FirstChildNode = childNode;
        }
    }

    void PrintTree(TreeNode* startNode, std::string levelDesignation = "----", bool isPrintIndexInFolder = false) {
        PrintOneNode(startNode, levelDesignation, 0, isPrintIndexInFolder);
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

    void RemoveNodeFromTree(int childNodeOrder, bool isRemoveNodeData = true) {
        TreeNode* currNode = GetChildNode(childNodeOrder);
        TreeNode* nextNode = currNode -> NextNeighbourNode;

        if (childNodeOrder == 0) {
            CurrentFolder -> FirstChildNode = nextNode;
        } else {
            GetChildNode(childNodeOrder - 1) -> NextNeighbourNode = nextNode;
        }

        if (isRemoveNodeData) {
            currNode -> NextNeighbourNode = nullptr;
            delete currNode;
        }
    }

    void CopyChildFolder(TreeNode* parentFolder, TreeNode* copyNode){
        TreeNode* newNode = new TreeNode{
            .ParentNode = parentFolder,
            .ElementType = copyNode -> ElementType
        };
        if (newNode -> ElementType == FileSystemFolder) {
            newNode -> Data = FolderInfo {
                .Name = std::get<FolderInfo>(copyNode -> Data).Name
            };
        } else {
            newNode -> Data = TextFileInfo {
                .Name = std::get<TextFileInfo>(copyNode -> Data).Name,
                .Text = std::get<TextFileInfo>(copyNode -> Data).Text
            };
        }

        Queue<TreeNode*>* allChildNodesQueue = new Queue<TreeNode*>(false);
        TreeNode* currChildNode = copyNode -> FirstChildNode;
        while (currChildNode != nullptr) {
            allChildNodesQueue->Enqueue(currChildNode);
            currChildNode = currChildNode -> NextNeighbourNode;
        }

        while (allChildNodesQueue->GetLength() > 0) {
            CopyChildFolder(newNode, allChildNodesQueue->Dequeue());
        }

        if (parentFolder -> FirstChildNode != nullptr) {

            TreeNode *lastNeighbourNode = parentFolder -> FirstChildNode;
            while (lastNeighbourNode->NextNeighbourNode != nullptr) {
                lastNeighbourNode = lastNeighbourNode->NextNeighbourNode;
            }
            lastNeighbourNode -> NextNeighbourNode = newNode;
        } else{
            parentFolder -> FirstChildNode = newNode;
        }
    }
};


#endif //SIMPLEFILESYSTEM_FILESYSTEMTREE_H
