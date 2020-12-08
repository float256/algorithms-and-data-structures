//
// Created by user on 30.11.2020.
//

#ifndef SIMPLEFILESYSTEM_FILESYSTEMPARSER_H
#define SIMPLEFILESYSTEM_FILESYSTEMPARSER_H


#include <regex>
#include "FileSystemTree.h"

class FileSystemParser {

private:
    static void Split(std::string inputString, char delimiter, Queue<std::string>* outputQueue) {
        std::string currString;

        for(char currSymbol: inputString) {
            if ((currSymbol == delimiter) && (!currString.empty())) {
                outputQueue -> Enqueue(currString);
                currString = "";
            } else {
                currString += currSymbol;
            }
        }
        if (!currString.empty()) {
            outputQueue -> Enqueue(currString);
        }
    }

    static bool IsCorrectString(std::string checkingString, std::string levelDesignation) {
        std::regex regex("(" + levelDesignation + ")+[^()]+([(][^()]+[)]\\s*)+$");

        return std::regex_match(checkingString, regex);
    }

    static int GetNestingLevel(std::string inputString, std::string levelDesignation) {
        std::regex regex("(" + levelDesignation + ")+");
        std::smatch allMatches;

        if (std::regex_search(inputString, allMatches, regex)) {
            return allMatches[0].length() / levelDesignation.length();
        } else {
            return 0;
        }
    }

    static Queue<std::string>* GetNodeData(std::string inputLine) {
        std::sregex_iterator emptyIterator = std::sregex_iterator();
        std::regex regexp("[^()]+");
        auto* result = new Queue<std::string>();

        for(std::sregex_iterator iter = std::sregex_iterator(inputLine.begin(), inputLine.end(), regexp);
            iter != emptyIterator;
            ++iter)
        {
            result -> Enqueue((*iter).str());
        }

        return result;
    }

public:
    static FileSystemTree* Parse(std::string fileSystemInFile, std::string levelDesignation="----") {
        std::unique_ptr<Queue<std::string>> allStringLines(new Queue<std::string>());
        FileSystemTree* fileSystemTree = new FileSystemTree();

        Split(fileSystemInFile, '\n', allStringLines.get());
        if (allStringLines -> GetLength() == 0) {
            return fileSystemTree;
        } else if (allStringLines -> Peek() != "/(root folder)") {
            throw std::invalid_argument("First line must always be '/ (root folder)'.");
        }

        int previousLevel = 1;
        int currLineIdx = 1;
        allStringLines -> Dequeue();
        while (allStringLines->GetLength() > 0) {
            std::string currLine = allStringLines -> Dequeue();
            if (IsCorrectString(currLine, levelDesignation)) {
                try {
                    int currLevel = GetNestingLevel(currLine, levelDesignation);

                    if (currLevel == 0) {
                        throw std::invalid_argument("Incorrect level indication");
                    }

                    std::string currLineWithoutLevelDesignation = currLine.substr(currLevel * levelDesignation.length());
                    std::unique_ptr<Queue<std::string>> currNodeParameters;
                    std::string currNodeType, currNodeName, currNodeText;
                    currNodeParameters.reset(GetNodeData(currLineWithoutLevelDesignation));

                    if (currNodeParameters -> GetLength() == 3) {
                        currNodeName = currNodeParameters -> Dequeue();
                        currNodeText = currNodeParameters -> Dequeue();
                        currNodeType = currNodeParameters -> Dequeue();
                    } else if (currNodeParameters -> GetLength() == 2) {
                        currNodeName = currNodeParameters -> Dequeue();
                        currNodeType = currNodeParameters -> Dequeue();
                    } else {
                        throw std::invalid_argument("Incorrect node parameters");
                    }

                    if (currLevel < previousLevel) {
                        for (int i = 0; i < previousLevel - currLevel; ++i) {
                            fileSystemTree -> CheckoutToParentNode();
                        }
                    } else if (currLevel > previousLevel) {
                        if (currLevel - previousLevel == 1) {
                            fileSystemTree -> CheckoutToChildNode(-1);
                        } else {
                            throw std::invalid_argument("The nesting level is incorrect");
                        }
                    }

                    if (currNodeType == "file") {
                        fileSystemTree->AddTextFile(currNodeName, currNodeText);
                    } else if ((currNodeType == "folder") || (currNodeType == "current folder")) {
                        fileSystemTree->AddFolder(currNodeName);
                    } else if (currNodeType == "root folder") {
                        throw std::invalid_argument("The current directory is declared two or more times");
                    }

                    previousLevel = currLevel;
                    ++currLineIdx;
                } catch (std::exception exception) {
                    throw std::invalid_argument("Parsing error at line number " + std::to_string(currLineIdx) + ": " + exception.what());
                }
            } else {
                throw std::invalid_argument("Incorrect line number " + std::to_string(currLineIdx));
            }
        }

        while (fileSystemTree -> GetCurrentFolder() != fileSystemTree -> GetRootFolder()) {
            fileSystemTree -> CheckoutToParentNode();
        }
        return fileSystemTree;
    }
};


#endif //SIMPLEFILESYSTEM_FILESYSTEMPARSER_H
