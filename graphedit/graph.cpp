#include <QDebug>
#include <algorithm>
#include "graph.h"

GraphEdge* Graph::getLowestEdge(QVector<GraphEdge*> edges)
{
    GraphEdge* lowest = nullptr;
    for (GraphEdge* edge : edges) {
        if (!(edge->node1->existInTrees && edge->node2->existInTrees) &&
            ((lowest == nullptr) || (edge->w <= lowest->w)))
        {
            lowest = edge;
        }
    }
    return lowest;
}

GraphEdge* Graph::getLowestEdge()
{
    GraphEdge* lowest = nullptr;
    for (GraphEdge& edge : edges) {
        if (!(edge.node1->existInTrees && edge.node2->existInTrees) &&
            ((lowest == nullptr) || (edge.w <= lowest->w)))
        {
            lowest = &edge;
        }
    }
    return lowest;
}

bool Graph::AddNode(float x, float y, int id)
{
    qDebug() << "Graph::AddNode(" << x << ", " << y << ", " << id << ")";

    if (id == 0)
    {
        id = 1;
        for (int i = 0; i < nodes.length(); i++)
        {
            id = std::max(id, nodes[i].id+1);
        }
    }

    GraphNode node(id, x, y);

    nodes.append(node);

    return true;
}

bool Graph::AddEdge(GraphNode* a, GraphNode* b, int w)
{
    if (a == b || a == nullptr || b == nullptr) return false;

    if (getEdgeByNodePair(a, b) == nullptr)
    {
        qDebug() << "Graph::AddEdge(" << a->id << ", " << b->id << ", " << w << ")";

        GraphEdge ed(a, b, w);
        edges.append(ed);
        return true;
    }
    else
    {
        return false;
    }
}

bool Graph::RemoveNode(GraphNode* node)
{
    if (node == nullptr)
        return false;

    if (begin == node) {
        begin = nullptr;
    } else if (end == node) {
        end = nullptr;
    }

    for (int i = 0; i < nodes.length(); i++)
    {
        if (&nodes[i] == node)
        {
            nodes.removeAt(i);
            break;
        }
    }

    int i = 0;
    while (i < edges.length())
    {
        if (edges[i].node1 == node || edges[i].node2 == node)
        {
            edges.remove(i);
        }
        else
        {
            i++;
        }
    }
    return true;
}

bool Graph::RemoveEdge(GraphEdge* edge)
{
    if (edge == nullptr)
        return false;

    for (int i = 0; i < edges.length(); i++)
    {
        if (&edges[i] == edge)
        {
            edges.removeAt(i);
            break;
        }
    }

    return true;
}

int Graph::GetNodeIndexByID(int id)
{
    for (int i = 0; i < nodes.length(); i++)
    {
        if (nodes[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

GraphEdge* Graph::getEdgeByNodePair(GraphNode* node1, GraphNode* node2)
{
    for (GraphEdge& edge : edges)
    {
        if ((edge.node1 == node1 && edge.node2 == node2) ||
            (edge.node1 == node2 && edge.node2 == node1)) {
            return &edge;
        }
    }

    return nullptr;
}

QVector<GraphEdge*> Graph::GetIncidentEdges(GraphNode* node)
{
    QVector<GraphEdge*> ret;

    for (int i = 0; i < edges.length(); i++)
    {
        if (edges[i].node1 == node || edges[i].node2 == node)
        {
            ret.push_back(&edges[i]);
        }
    }

    return ret;
}

GraphNode* Graph::GetNodeByID(int id)
{
    for (int i = 0; i < nodes.length(); i++)
    {
        if (nodes[i].id == id)
        {
            return &nodes[i];
        }
    }
    return nullptr;
}

void Graph::setBeginNode(GraphNode* node)
{
    begin = node;
    if (end == begin) {
        end = nullptr;
    }
}

void Graph::setEndNode(GraphNode* node)
{
    end = node;
    if (begin == end) {
        begin = nullptr;
    }
}

void Graph::colorGreenToBold()
{
    for (GraphEdge& edge : edges) {
        if (edge.color == ColorMode::GREEN) {
            edge.color = ColorMode::BOLD;
        }
    }
}

Graph& Graph::operator=(const Graph &src)
{
    qDebug() << "Graph::operator=";
    nodes = src.nodes;
    edges = src.edges;
    directed = src.directed;

    begin = (src.begin != nullptr) ? GetNodeByID(src.begin->id) : nullptr;
    end = (src.end != nullptr) ? GetNodeByID(src.end->id) : nullptr;

    for (auto& edge : edges) {
        edge.node1 = GetNodeByID(edge.node1->id);
        edge.node2 = GetNodeByID(edge.node2->id);
    }

    return *this;
}

bool GraphEdge::lessThan(const GraphEdge *e1, const GraphEdge *e2)
{
    return e1->w < e2->w;
}
