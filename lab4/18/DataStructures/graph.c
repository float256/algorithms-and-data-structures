//
// Created by user on 12.12.2020.
//

#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"

// Private functions

int deleteElementFromArray(void **startArrayPtr, void *elementForDeleting, int arrayLength) {
    if (startArrayPtr != NULL) {
        bool isFindDeletingElement = false;

        for (int i = 0; i < arrayLength; ++i) {
            if (startArrayPtr[i] == elementForDeleting) {
                isFindDeletingElement = true;
                continue;
            } else if (isFindDeletingElement) {
                startArrayPtr[i - 1] = startArrayPtr[i];
            }
        }

        if (isFindDeletingElement) {
            free(startArrayPtr[arrayLength - 1]);
            free(elementForDeleting);
            return arrayLength - 1;
        } else {
            return arrayLength;
        }
    } else {
        return 0;
    }
}

void addElementToEndOfArray(void **startArrayPtr, void *elementForAdding, int arrayLength, int oneElementSize) {
    *startArrayPtr = realloc(startArrayPtr, oneElementSize * (arrayLength + 1));
    startArrayPtr[arrayLength] = elementForAdding;
}

// Public functions

GraphNode* CreateNode (int nodeNumber) {
    GraphNode* newNode = (GraphNode*) malloc(sizeof(GraphNode));
    newNode -> NodeNumber = nodeNumber;
    newNode -> NeighbourNodes = NULL;
    newNode -> NumberOfNeighbourNodes = 0;
    return newNode;
};

GraphEdge* ConnectTwoGraphNodes(GraphNode* firstNode, GraphNode* secondNode, int edgeWeight) {
    GraphEdge* newEdge = (GraphEdge*) malloc(sizeof(GraphEdge));
    newEdge -> FirstNode = firstNode;
    newEdge -> SecondNode = secondNode;
    newEdge -> Weight = edgeWeight;

    addElementToEndOfArray((void*) firstNode -> NeighbourNodes, newEdge, firstNode -> NumberOfNeighbourNodes, sizeof(GraphEdge));
    firstNode -> NumberOfNeighbourNodes++;

    addElementToEndOfArray((void*) secondNode -> NeighbourNodes, newEdge, secondNode -> NumberOfNeighbourNodes, sizeof(GraphEdge));
    secondNode -> NumberOfNeighbourNodes++;
    return newEdge;
}

void DeleteGraphEdge(GraphEdge* edgeForDeleting) {
    deleteElementFromArray(
            (void *)edgeForDeleting -> FirstNode -> NeighbourNodes,
            edgeForDeleting,
            edgeForDeleting -> FirstNode -> NumberOfNeighbourNodes);
    edgeForDeleting -> FirstNode -> NumberOfNeighbourNodes -= 1;

    deleteElementFromArray(
            (void *)edgeForDeleting -> SecondNode -> NeighbourNodes,
            edgeForDeleting,
            edgeForDeleting -> SecondNode -> NumberOfNeighbourNodes);
    edgeForDeleting -> SecondNode -> NumberOfNeighbourNodes -= 1;

    free(edgeForDeleting);
}

void DeleteNode(GraphNode* nodeForDeleting) {
    for (int i = 0; i < nodeForDeleting->NumberOfNeighbourNodes; ++i) {
        DeleteGraphEdge(nodeForDeleting -> NeighbourNodes[i]);
    }

    free(nodeForDeleting);
}

void AddNodeToGraph(Graph* graph, GraphNode* graphNode) {
    addElementToEndOfArray((void **)graph -> AllNodesArrayPtr, graphNode, graph -> NumberOfAllNodes, sizeof(GraphNode));
    graph -> NumberOfAllNodes++;
}

void DeleteNodeFromGraph(Graph* graph, GraphNode* nodeForDeleting) {
    deleteElementFromArray(graph -> NumberOfAllNodes, nodeForDeleting, graph -> NumberOfAllNodes);
}
