//
// Created by user on 01.01.2021.
//

#include "GraphParser.h"

Queue<std::string> &GraphParser::Split(const std::string &inputString, char delimiter) {
    std::string currString;
    Queue<std::string>& outputQueue = *(new Queue<std::string>());

    for(char currSymbol: inputString) {
        if ((currSymbol == delimiter) && (!currString.empty())) {
            outputQueue.Enqueue(currString);
            currString = "";
        } else if (currSymbol != delimiter) {
            currString += currSymbol;
        }
    }
    if (!currString.empty()) {
        outputQueue.Enqueue(currString);
    }
    return outputQueue;
}

std::vector<std::string> &GraphParser::getAllNodesNames(const std::string &firstStringInFile) {
    std::unique_ptr<Queue<std::string>> allGraphNodeNamesQueue(&(Split(firstStringInFile, ' ')));
    std::vector<std::string>& allGraphNodeNames = *(new std::vector<std::string>());

    while(allGraphNodeNamesQueue -> GetLength() > 0) {
        allGraphNodeNames.push_back(allGraphNodeNamesQueue -> Dequeue());
    }
    return allGraphNodeNames;
}

int GraphParser::GetNumberOfNodes(const std::string &graphInString) {
    std::unique_ptr<Queue<std::string>> allStringLines(&(Split(graphInString, '\n')));
    std::unique_ptr<Queue<std::string>> splittedFirstLine(&(Split(allStringLines->Peek(), ' ')));
    return splittedFirstLine -> GetLength();
}

Graph &GraphParser::Parse(std::string graphInString) {
    std::unique_ptr<Queue<std::string>> allStringLines(&(Split(graphInString, '\n')));
    Graph& parsedGraph = *(new Graph(GetNumberOfNodes(graphInString)));

    parsedGraph.SetNodeNames(getAllNodesNames(allStringLines -> Dequeue()));
    while(allStringLines -> GetLength() > 0) {
        std::string firstNodeName, secondNodeName;
        int weight;

        std::string currString = allStringLines->Dequeue();
        std::unique_ptr<Queue<std::string>> currStringParts(&(Split(currString, ' ')));
        if (currStringParts -> GetLength() != 3) {
            throw std::invalid_argument("Некорректный формат файла");
        }

        firstNodeName = currStringParts -> Dequeue();
        secondNodeName = currStringParts -> Dequeue();
        if (firstNodeName == secondNodeName) {
            continue;
        }
        try {
            weight = std::stoi(currStringParts -> Dequeue());
            if (weight < 0) {
                throw std::invalid_argument("Вес ребра не может быть отрицательным");
            } else if (weight == 0) {
                throw std::invalid_argument("Вес ребра не может равен нулю");
            }
        } catch(const std::exception& e) {
            std::string message = "Ошибка при парсинге числа: ";
            message += e.what();

            throw std::invalid_argument(message);
        }

        parsedGraph.SetEdge(firstNodeName, secondNodeName, weight);
    }

    return parsedGraph;
}
