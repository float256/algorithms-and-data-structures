//
// Created by user on 12.12.2020.
//

#ifndef DIJKSTRAALGORITHM_GRAPH_H
#define DIJKSTRAALGORITHM_GRAPH_H

struct node {
    int NodeNumber;
    struct graphEdge {
        struct node* FirstNode;
        struct node* SecondNode;
        int Weight;
    } **NeighbourNodes;
    int NumberOfNeighbourNodes;
};

struct graph {
    struct node** AllNodesArrayPtr;
    int NumberOfAllNodes;
};
typedef struct graph Graph;
typedef struct node GraphNode;
typedef struct graphEdge GraphEdge;

GraphNode* CreateNode (int nodeNumber);
GraphEdge* ConnectTwoGraphNodes(GraphNode* firstNode, GraphNode* secondNode, int edgeWeight);
void DeleteGraphEdge(GraphEdge* edgeForDeleting);
void DeleteNode(GraphNode* nodeForDeleting);

#endif //DIJKSTRAALGORITHM_GRAPH_H
