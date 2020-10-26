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

    void PrintOneNode(TreeNode* currNode, TreeNode* skippedNodes, int skippedNodesLength, std::string levelDesignation, int currLevel) {
        if (currNode != nullptr) {
            for (int i = 0; i < currLevel; ++i) {
                std::cout << levelDesignation;
            }
            std::variant<TextFileInfo, FolderInfo> currNodeInfo = currNode->Data;
            if (currNode->ElementType == FileSystemFolder) {
                std::cout << std::get<FolderInfo>(currNodeInfo).Name << " (file)";
            } else {
                std::cout << std::get<TextFileInfo>(currNodeInfo).Name << " (text)";
            }
        }
    }


public:
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
        ++Count;
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
        ++Count;
    }

    void CheckoutToChildNode(int childNodeOrder) {
        TreeNode* currChildNode = CurrentFolder -> FirstChildNode;
        if (childNodeOrder > Count - 1) {
            throw std::out_of_range("Out of range when trying to get child node");
        }
        for (int i = 0; i < childNodeOrder; ++i) {
            currChildNode = currChildNode -> NextNeighbourNode;
        }
        if (currChildNode -> ElementType != FileSystemFolder) {
            throw std::invalid_argument("Node with specified order is not a folder");
        }
        CurrentFolder = currChildNode;
    }

    void CheckoutToParentNode() {
        if (CurrentFolder -> ParentNode != nullptr) {
            CurrentFolder = CurrentFolder -> ParentNode;
        } else {
            throw std::out_of_range("Current folder hasn't parent folder");
        }
    }

    void AddExistingNode(TreeNode* node){
        TreeNode* previousChildNode = CurrentFolder -> FirstChildNode;
        node -> NextNeighbourNode = CurrentFolder -> NextNeighbourNode;
        CurrentFolder -> NextNeighbourNode = previousChildNode;
    }

    void PrintTree(TreeNode* skippedNodes, int skippedNodesLength, std::string levelDesignation = "//") {

    }
};


#endif //SIMPLEFILESYSTEM_FILESYSTEMTREE_H
