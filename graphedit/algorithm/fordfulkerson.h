#ifndef FORDFULKERSON_H
#define FORDFULKERSON_H

#include "algorithm.h"

class FordFulkerson : public Algorithm
{
    static const int WHITE = 0;
    static const int GRAY = 1;
    static const int BLACK = 2;

    int n;
    int** capacity;
    int** flow;
    int* color;
    int* pred;

    int head, tail;
    int* q;

    void enqueue(int x);
    int dequeue();
    int bfs(int start, int target);
    int maxFlow(int source, int sink);
    int getNodeIdx(GraphNode* node);

public:
    FordFulkerson(QList<Graph>* states);
    ~FordFulkerson();
    bool run();
};

#endif // FORDFULKERSON_H
