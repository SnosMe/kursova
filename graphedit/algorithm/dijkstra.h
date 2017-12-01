#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "algorithm.h"

struct DijkstraGraph
{
    QVector<GraphNode*> nodes;
    GraphNode* end;
};

class Dijkstra : Algorithm
{
public:
    Dijkstra(QList<Graph>* states);
    bool run();
};

#endif // DIJKSTRA_H
