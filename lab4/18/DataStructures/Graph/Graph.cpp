//
// Created by user on 01.01.2021
//

#include <stdexcept>
#include "Graph.h"


template<class T>
int Graph::findIndex(std::vector<T> array, T element) {
    int idx = -1;

    for (int i = 0; i < array.size(); ++i) {
        if (array[i] == element) {
            idx = i;
            break;
        }
    }
    return idx;
}

int Graph::getNodeIdx(std::string nodeName) {
    int nodeAdjacencyMatrixIdx = findIndex<std::string>(_allNodeNames, nodeName);

    if (nodeAdjacencyMatrixIdx == -1) {
        throw std::invalid_argument("Node named '" + nodeName  +"' was not found");
    }

    return nodeAdjacencyMatrixIdx;
}

std::vector<std::string> Graph::GetNodeNames() {
    return _allNodeNames;
}

int Graph::GetEdgeWeight(std::string firstNodeName, std::string secondNodeName) {
    int firstNodeAdjacencyMatrixIdx = getNodeIdx(firstNodeName);
    int secondNodeAdjacencyMatrixIdx = getNodeIdx(secondNodeName);

    return _adjacencyMatrix[firstNodeAdjacencyMatrixIdx][secondNodeAdjacencyMatrixIdx];
}

std::vector<int> Graph::GetAllEdges(std::string nodeName) {
    int nodeAdjacencyMatrixIdx = getNodeIdx(nodeName);
    return _adjacencyMatrix[nodeAdjacencyMatrixIdx];
}

int Graph::GetNumberOfNodes() {
    return _allNodeNames.size();
}

void Graph::SetNodeNames(std::vector<std::string> newNodeNames) {
    if (GetNumberOfNodes() == newNodeNames.size()){
        _allNodeNames = newNodeNames;
    } else {
        throw std::invalid_argument("The length of the array with new node names does not match the number of nodes");
    }
}

void Graph::SetEdge(std::string firstNodeName, std::string secondNodeName, int weight) {
    int firstNodeAdjacencyMatrixIdx = getNodeIdx(firstNodeName);
    int secondNodeAdjacencyMatrixIdx = getNodeIdx(secondNodeName);

    _adjacencyMatrix[firstNodeAdjacencyMatrixIdx][secondNodeAdjacencyMatrixIdx] = weight;
}

void Graph::DeleteEdge(std::string firstNodeName, std::string secondNodeName) {
    int firstNodeAdjacencyMatrixIdx = getNodeIdx(firstNodeName);
    int secondNodeAdjacencyMatrixIdx = getNodeIdx(secondNodeName);

    _adjacencyMatrix[firstNodeAdjacencyMatrixIdx][secondNodeAdjacencyMatrixIdx] = 0;
}

bool Graph::IsExist(const std::string &nodeName) {
    for (const auto& currNodeName: _allNodeNames) {
        if (currNodeName == nodeName) {
            return true;
        }
    }
    return false;
}
