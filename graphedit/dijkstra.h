#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

struct DijkstraGraph
{
    QVector<GraphNode*> nodes;
    GraphNode* end;
};

#endif // DIJKSTRA_H
