#include "algorithm.h"
#include <QMessageBox>

bool Algorithm::checkRequirements(Requirements flags)
{
    if (gCopy.edges.size() < 1) {
        showError("Граф занадто малий!");
        return false;
    }
    if (flags & BeginEndConnected)
    {
        if (gCopy.begin == nullptr &&
            gCopy.end == nullptr) {
            showError("Початок і кінець графу не встановлено!");
            return false;
        } else if (gCopy.begin == nullptr) {
            showError("Початок графу не встановлено!");
            return false;
        } else if (gCopy.end == nullptr) {
            showError("Кінець графу не встановлено!");
            return false;
        } else if (!isBeginEndConnected()) {
            showError("Вершини початку та кінця не з'єднанні!");
            return false;
        }
    }
    if ((flags & Directed) && !gCopy.directed) {
        showError("Алгоритм застосовується лише до орієнтованих графів!");
        return false;
    }
    if ((flags & Undirected) && gCopy.directed) {
        showError("Алгоритм застосовується лише до НЕ орієнтованих графів!");
        return false;
    }

    return true;
}

void Algorithm::showError(const QString &msg)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/img/app.ico"));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("JustGraphIT");
    msgBox.setText(msg);
    msgBox.exec();
}

bool Algorithm::isBeginEndConnected()
{
    QList<GraphNode*> queue;
    Graph savedState = gCopy;

    gCopy.begin->existInTrees = true;
    queue.append(gCopy.begin);

        while (!queue.empty())
        {
            GraphNode* s = queue.first();
            queue.removeFirst();

            QVector<GraphEdge*> edges = gCopy.GetIncidentEdges(s);
            for (GraphEdge* edge : edges)
            {
                GraphNode* node = (edge->node1 == s) ? edge->node2 : edge->node1;

                if (node == gCopy.end) {
                    // restore graph
                    gCopy = savedState;
                    return true;
                }
                else if (!node->existInTrees)
                {
                    node->existInTrees = true;
                    queue.append(node);
                }
            }
        }

    // restore graph
    gCopy = savedState;
    return false;
}

Algorithm::Algorithm(QList<Graph> *states)
{
    this->states = states;
    gCopy = states->at(0);
    result = 0;
}

int Algorithm::getResult()
{
    return result;
}
