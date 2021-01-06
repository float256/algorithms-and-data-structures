//
// Created by user on 03.01.2021.
//

#ifndef DIJKSTRAALGORITHM_DIJKSTRAALGORITHM_H
#define DIJKSTRAALGORITHM_DIJKSTRAALGORITHM_H

#include <vector>
#include <fstream>
#include "Entities/AlgorithmHistory.h"
#include "../DataStructures/Graph/Graph.h"
#include "../DataStructures/Stack/Stack.h"


class DijkstraAlgorithm {
private:
    static int selectLabel(AlgorithmState algorithmState);

    static Stack<std::string> &findPathToNode(const NodeLabel& searchingNodeLabel, const AlgorithmState& state);

public:
    static AlgorithmHistory FindAllShortPathsWithHistory(Graph graph, const std::string &startPoint);

    static void PrintHistoryToFile(AlgorithmHistory history, std::string outputFileName, std::string startNodeName);
};


#endif //DIJKSTRAALGORITHM_DIJKSTRAALGORITHM_H
