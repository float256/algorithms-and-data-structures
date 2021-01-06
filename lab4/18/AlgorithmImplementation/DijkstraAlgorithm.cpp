//
// Created by user on 03.01.2021.
//

#include "DijkstraAlgorithm.h"

int DijkstraAlgorithm::selectLabel(AlgorithmState algorithmState) {
    int selectedLabelIdx = -1;
    for (int i = 0; i < algorithmState.AllLabels.size(); ++i) {
        bool isSelectCurrLabel =
                (algorithmState.AllLabels[i].Weight != -1) &&
                (!algorithmState.AllLabels[i].IsFinalLabel) && (
                        (selectedLabelIdx == -1) ||
                        (algorithmState.AllLabels[selectedLabelIdx].Weight >= algorithmState.AllLabels[i].Weight)
                );
        if (isSelectCurrLabel){
            selectedLabelIdx = i;
        }
    }

    if (algorithmState.AllLabels[selectedLabelIdx].IsFinalLabel) {
        return -1;
    } else {
        return selectedLabelIdx;
    }
}

Stack<std::string> &DijkstraAlgorithm::findPathToNode(const NodeLabel &searchingNodeLabel, const AlgorithmState &state) {
    Stack<std::string> &pathToNode = *(new Stack<std::string>);
    NodeLabel currNodeLabelInPath;

    if (searchingNodeLabel.PreviousNodeName.empty()) {
        return pathToNode;
    }

    pathToNode.Push(searchingNodeLabel.NodeName);
    currNodeLabelInPath = searchingNodeLabel;
    while (!currNodeLabelInPath.PreviousNodeName.empty()) {
        for (const auto& currNodeLabel: state.AllLabels) {
            if (currNodeLabel.NodeName == currNodeLabelInPath.PreviousNodeName) {
                currNodeLabelInPath = currNodeLabel;
                break;
            }
        }
        pathToNode.Push(currNodeLabelInPath.NodeName);
    }
    return pathToNode;
}

AlgorithmHistory DijkstraAlgorithm::FindAllShortPathsWithHistory(Graph graph, const std::string &startPoint) {
    AlgorithmHistory history = AlgorithmHistory {};
    AlgorithmState currAlgorithmState;
    std::vector<std::string> allNodeNames = graph.GetNodeNames();

    if (!graph.IsExist(startPoint)) {
        throw std::invalid_argument("Стартовый узел с указанным именем не содержится в графе");
    }

    currAlgorithmState = AlgorithmState {};
    for (int i = 0; i < graph.GetNumberOfNodes(); ++i) {
        currAlgorithmState.AllLabels.push_back(NodeLabel {
                .NodeName = allNodeNames[i],
                .PreviousNodeName = "",
                .Weight = -1,
                .IsFinalLabel = false,
        });
        if (allNodeNames[i] == startPoint) {
            currAlgorithmState.AllLabels.back().Weight = 0;
        }
    }
    history.AllStates.push_back(currAlgorithmState);

    bool isAllLabelsAreFinal = false;
    while(!isAllLabelsAreFinal) {
        int currLabelIdx = selectLabel(currAlgorithmState);
        isAllLabelsAreFinal = (currLabelIdx == -1);

        if (!isAllLabelsAreFinal) {
            NodeLabel selectedLabel = currAlgorithmState.AllLabels[currLabelIdx];
            currAlgorithmState.AllLabels[currLabelIdx].IsFinalLabel = true;
            std::vector<int> selectedLabelEdges = graph.GetAllEdges(selectedLabel.NodeName);
            for (int i = 0; i < selectedLabelEdges.size(); ++i) {
                int newEdgeWeight = selectedLabelEdges[i];
                int currEdgeWeight = currAlgorithmState.AllLabels[i].Weight;

                if (
                        ((currEdgeWeight == -1) || ((newEdgeWeight + selectedLabel.Weight) < currEdgeWeight)) &&
                        (newEdgeWeight != 0)
                        ) {
                    currAlgorithmState.AllLabels[i].PreviousNodeName = selectedLabel.NodeName;
                    currAlgorithmState.AllLabels[i].Weight = newEdgeWeight + selectedLabel.Weight;
                }
            }
            history.AllStates.push_back(currAlgorithmState);
        }
    }
    return history;
}

void DijkstraAlgorithm::PrintHistoryToFile(AlgorithmHistory history, std::string outputFileName, std::string startNodeName) {
    std::ofstream outputFile;
    outputFile.open(outputFileName);
    if (!outputFile) {
        throw std::invalid_argument("Выходной файл невозможно создать");
    }

    outputFile << "Стартовый узел: " << startNodeName << std::endl
               << std::endl
               << "Шаги выполнения алгоритма:" << std::endl;
    for (int i = 0; i < history.AllStates.size(); ++i) {
        outputFile << "Шаг №" << (i + 1) << std::endl;
        for (const auto& currLabel: history.AllStates[i].AllLabels) {
            outputFile << "    " << currLabel.NodeName;
            if (!currLabel.PreviousNodeName.empty()) {
                outputFile << " (" << currLabel.PreviousNodeName << ") ";
            }
            outputFile << "    Длина пути: ";
            if (currLabel.Weight == -1) {
                outputFile << "Не определена";
            } else {
                outputFile << currLabel.Weight;
            }
            if (currLabel.IsFinalLabel) {
                outputFile << "    Окончательная метка";
            }
            outputFile << std::endl;
        }
    }

    outputFile << std::endl
               << "Найденные пути:" << std::endl;
    AlgorithmState lastState = history.AllStates.back();
    for(const auto& currLabel: lastState.AllLabels) {
        if (currLabel.NodeName == startNodeName) {
            continue;
        }

        Stack<std::string> pathToNode = findPathToNode(currLabel, lastState);

        outputFile << "    " << currLabel.NodeName << ": ";
        if (pathToNode.GetLength() == 0) {
            outputFile << "Узел недостижим из узла '" << startNodeName << "'";
        } else {
            while (pathToNode.GetLength() > 0) {
                outputFile << pathToNode.Pop();
                if (pathToNode.GetLength() != 0) {
                    outputFile << " -> ";
                }
            }
        }
        outputFile << std::endl;
    }
}
