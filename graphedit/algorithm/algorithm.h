#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "mainwindow.h"

class Algorithm
{
protected:
    QList<Graph>* states;
    Graph gCopy;

public:
    Algorithm(QList<Graph>* states)
    {
        this->states = states;
        gCopy = (*states)[0];
    }

    virtual bool run() = 0;
};

#endif // ALGORITHM_H
