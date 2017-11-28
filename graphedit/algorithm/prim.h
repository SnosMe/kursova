#ifndef PRIM_H
#define PRIM_H

#include "algorithm.h"

class Prim : Algorithm
{
    void makeColored(GraphEdge* edge);

public:
    Prim(QList<Graph>* states);
    bool run();
};

#endif // PRIM_H
