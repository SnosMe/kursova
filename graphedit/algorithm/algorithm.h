#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "mainwindow.h"

class Algorithm
{
public:
    Algorithm(QList<Graph>* states);
    virtual bool run() = 0;
    int getResult();

    enum Requirement
    {
        NoRequirements = 0,
        Directed = 1,
        Undirected = 2,
        BeginEndConnected = 4
    };
    Q_DECLARE_FLAGS(Requirements, Requirement)

protected:
    QList<Graph>* states;
    Graph gCopy;
    int result;
    bool checkRequirements(Requirements flags = NoRequirements);

private:
    void showError(const QString& msg);
    bool isBeginEndConnected();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Algorithm::Requirements)

#endif // ALGORITHM_H
