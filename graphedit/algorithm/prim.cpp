#include "prim.h"

bool Prim::run()
{
    GraphEdge* firstEdge = gCopy.getLowestEdge()[0];
    makeColored(firstEdge);
    states->append(gCopy);

    for (int i = 0; i < gCopy.nodes.size(); i++)
    {
        GraphEdge* lowestEdge = nullptr;

        for (GraphNode& node : gCopy.nodes)
        {
            if (!node.existInTrees)
                continue;

            for (GraphEdge* edge : gCopy.getLowestEdge(gCopy.GetIncidentEdges(&node)))
            {
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
            makeColored(lowestEdge);
            states->append(gCopy);
        }
    }

    return true;
}

void Prim::makeColored(GraphEdge* edge)
{
    edge->node1->existInTrees = true;
    edge->node2->existInTrees = true;
    edge->node1->color = ColorMode::BOLD;
    edge->node2->color = ColorMode::BOLD;
    edge->color = ColorMode::BOLD;
}

Prim::Prim(QList<Graph>* states)
    : Algorithm(states)
{
}
