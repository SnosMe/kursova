#ifndef PRIM_H
#define PRIM_H

#include "algorithm.h"

class Prim : public Algorithm
{
    void markEdge(GraphEdge* edge);

public:
    Prim(QList<Graph>* states);
    bool run();
};

#endif // PRIM_H
