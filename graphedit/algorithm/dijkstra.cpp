#include "dijkstra.h"


Dijkstra::Dijkstra(QList<Graph> *states)
    : Algorithm(states)
{
}

bool Dijkstra::run()
{
    if (!checkRequirements(BeginEndConnected | Undirected)) {
        return false;
    }

    QVector<DijkstraGraph> dGraphArr;
    DijkstraGraph dGraph;
    GraphNode* startVert = gCopy.begin;

    GraphEdge* edge = Graph::getLowestEdge(gCopy.GetIncidentEdges(startVert));
    GraphNode* end = (edge->node1 == startVert) ? edge->node2 : edge->node1;
    end->length = edge->w;
    edge->node1->existInTrees = true;
    edge->node2->existInTrees = true;
    dGraph.nodes.append(edge->node1);
    dGraph.nodes.append(edge->node2);
    dGraph.end = end;
    dGraphArr.append(dGraph);

    startVert->color = ColorMode::BOLD;
    states->append(gCopy);

    end->color = ColorMode::BOLD;
    edge->color = ColorMode::BOLD;
    states->append(gCopy);

    if (end == gCopy.end) {
        edge->node1->color = ColorMode::GREEN;
        edge->node2->color = ColorMode::GREEN;
        edge->color = ColorMode::GREEN;
        states->append(gCopy);
        result = edge->w;
        return true;
    }

    while (true) {
        GraphEdge* lowestEdge = nullptr;
        int lowestLength = INT_MAX;
        DijkstraGraph* foundInGraph = nullptr;
        for (DijkstraGraph& dGraph : dGraphArr) {
            for (GraphNode* node : dGraph.nodes) {
                GraphEdge* edge = Graph::getLowestEdge(gCopy.GetIncidentEdges(node));
                if (edge != nullptr) {
                    if (edge->node1->existInTrees && edge->node2->existInTrees)
                        continue;
                    int pathLen = (edge->node1->length) ? (edge->node1->length + edge->w) : (edge->node2->length + edge->w);
                    if ((edge->node1 == startVert) || (edge->node2 == startVert)) pathLen = edge->w;
                    if ((lowestEdge == nullptr) || (pathLen < lowestLength)) {
                        lowestEdge = edge;
                        lowestLength = pathLen;
                        foundInGraph = &dGraph;
                    }
                }
            }
        }

        GraphNode* end = (lowestEdge->node1->existInTrees) ? lowestEdge->node2 : lowestEdge->node1;
        GraphNode* prev = (lowestEdge->node1->existInTrees) ? lowestEdge->node1 : lowestEdge->node2;
        if ((foundInGraph->end != prev) || (lowestEdge->node1 == startVert) || (lowestEdge->node2 == startVert)) {
            if ((lowestEdge->node1 == startVert) || (lowestEdge->node2 == startVert)) {
                dGraphArr.append(DijkstraGraph());
                foundInGraph = &dGraphArr.last();
                foundInGraph->nodes.append(startVert);
            } else {
                dGraphArr.append(DijkstraGraph(*foundInGraph));
                foundInGraph = &dGraphArr.last();
                for (int i = foundInGraph->nodes.size(); i != 1; i--) {
                    if (foundInGraph->nodes[i-1] != prev) {
                        foundInGraph->nodes.removeLast();
                    } else {
                        break;
                    }
                }
            }
        }
        foundInGraph->nodes.append(end);
        foundInGraph->end = end;
        end->length = lowestLength;
        end->existInTrees = true;

        lowestEdge->color = ColorMode::BOLD;
        end->color = ColorMode::BOLD;
        states->append(gCopy);

        qDebug() << ": (" << lowestEdge->node1->id << ", " << lowestEdge->node2->id << ")\n";
        if (end == gCopy.end) {
            GraphNode* prev = nullptr;
            for (GraphNode* node : foundInGraph->nodes) {
                node->color = ColorMode::GREEN;
                if (prev != nullptr) {
                    for (GraphEdge& edge : gCopy.edges) {
                        if ((edge.node1 == prev && edge.node2 == node) ||
                            (edge.node1 == node && edge.node2 == prev)) {
                            edge.color = ColorMode::GREEN;
                            result += edge.w;
                        }
                    }
                }
                prev = node;
            }
            states->append(gCopy);
            break;
        }
    }

    return true;
}
