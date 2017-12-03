#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "algorithm.h"

class Kruskal : public Algorithm
{
public:
    Kruskal(QList<Graph>* states);
    bool run();
};

#endif // KRUSKAL_H
