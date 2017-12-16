#include "prim.h"

bool Prim::run()
{
    if (!checkRequirements(Undirected)) {
        return false;
    }

    GraphEdge* firstEdge = gCopy.getLowestEdge();
    markEdge(firstEdge);
    states->append(gCopy);

    for (int i = 0; i < gCopy.nodes.size(); i++)
    {
        GraphEdge* lowestEdge = nullptr;

        for (GraphNode& node : gCopy.nodes)
        {
            if (!node.existInTrees)
                continue;

            GraphEdge* edge = Graph::getLowestEdge(gCopy.GetIncidentEdges(&node));
            if (edge != nullptr) {
                if (edge->node1->existInTrees && edge->node2->existInTrees)
                    continue;
                else if (lowestEdge == nullptr)
                    lowestEdge = edge;
                else if (edge->w < lowestEdge->w)
                    lowestEdge = edge;
            }
        }

        if (lowestEdge != nullptr)
        {
            markEdge(lowestEdge);
            states->append(gCopy);
        }
    }

    return true;
}

void Prim::markEdge(GraphEdge* edge)
{
    edge->node1->existInTrees = true;
    edge->node2->existInTrees = true;
    edge->node1->color = ColorMode::BOLD;
    edge->node2->color = ColorMode::BOLD;
    edge->color = ColorMode::BOLD;
    result += edge->w;
}

Prim::Prim(QList<Graph>* states)
    : Algorithm(states)
{
}
