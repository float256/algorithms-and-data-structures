//
// Created by user on 03.01.2021.
//

#ifndef DIJKSTRAALGORITHM_NODELABEL_H
#define DIJKSTRAALGORITHM_NODELABEL_H


#include <string>

struct NodeLabel {
    std::string NodeName;
    std::string PreviousNodeName;
    int Weight;
    bool IsFinalLabel;
};


#endif //DIJKSTRAALGORITHM_NODELABEL_H
