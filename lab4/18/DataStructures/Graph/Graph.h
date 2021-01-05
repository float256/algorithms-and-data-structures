//
// Created by user on 01.01.2021.
//

#ifndef DIJKSTRAALGORITHM_GRAPH_H
#define DIJKSTRAALGORITHM_GRAPH_H


#include <string>
#include <array>
#include <vector>

class Graph{
private:
    std::vector<std::string> _allNodeNames;
    std::vector<std::vector<int>> _adjacencyMatrix;

    template<class T>
    int findIndex(std::vector<T> array, T element);

    int getNodeIdx(std::string nodeName);

public:
    Graph(int numberOfNodes){
        _adjacencyMatrix.resize(numberOfNodes);
        for (int i = 0; i < numberOfNodes; ++i){
            _adjacencyMatrix[i].resize(numberOfNodes);
        }

        _allNodeNames.resize(numberOfNodes);
    }

    std::vector<std::string> GetNodeNames();

    int GetEdgeWeight(std::string firstNodeName, std::string secondNodeName);

    std::vector<int> GetAllEdges(std::string nodeName);

    int GetNumberOfNodes();

    void SetNodeNames(std::vector<std::string> newNodeNames);

    void SetEdge(std::string firstNodeName, std::string secondNodeName, int weight);

    void DeleteEdge(std::string firstNodeName, std::string secondNodeName);
};



#endif //DIJKSTRAALGORITHM_GRAPH_H
