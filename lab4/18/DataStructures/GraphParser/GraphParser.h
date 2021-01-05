//
// Created by user on 01.01.2021.
//

#ifndef DIJKSTRAALGORITHM_GRAPHPARSER_H
#define DIJKSTRAALGORITHM_GRAPHPARSER_H

#include <memory>
#include "../Graph/Graph.h"
#include "../Queue.h"

class GraphParser {
private:
    static Queue<std::string>& Split(const std::string &inputString, char delimiter);

    static std::vector<std::string>& getAllNodesNames(const std::string& firstStringInFile);

    static int GetNumberOfNodes(const std::string &graphInString);

public:
    static Graph& Parse(std::string graphInString);
};


#endif //DIJKSTRAALGORITHM_GRAPHPARSER_H
