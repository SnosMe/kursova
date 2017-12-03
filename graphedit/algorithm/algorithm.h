#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "mainwindow.h"

class Algorithm
{
protected:
    QList<Graph>* states;
    Graph gCopy;
    int result;

public:
    Algorithm(QList<Graph>* states)
    {
        this->states = states;
        gCopy = (*states)[0];
        result = 0;
    }

    virtual bool run() = 0;

    int getResult()
    {
        return result;
    }
};

#endif // ALGORITHM_H
