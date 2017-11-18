#include <QDebug>
#include <algorithm>
#include "graph.h"

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

    GraphNode nd(id, x, y, false);

    nodes.append(nd);

    return true;
}

bool Graph::AddEdge(GraphNode* a, GraphNode* b, int w)
{
    if (a == b || a == nullptr || b == nullptr) return false;

    bool found = false;
    for (int i = 0; i < edges.length(); i++)
    {
        if ((edges[i].node1 == a && edges[i].node2 == b) ||
            (edges[i].node1 == b && edges[i].node2 == a))
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        qDebug() << "Graph::AddEdge(" << a->id << ", " << b->id << ", " << w << ")";

        GraphEdge ed(a, b, w, false);
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

    for (int i = 0; i < nodes.length(); i++)
    {
        if (&nodes[i] == node)
        {
            nodes.remove(i);
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

/* bool Graph::RemoveEdge(int a, int b)
{
    bool found = false;
    int i = 0;

    while (i < edges.length())
    {
        if ((edges[i].node1 == a &&
             edges[i].node2 == b) ||
                (edges[i].node1 == b &&
                 edges[i].node2 == a))
        {
            edges.remove(i);
            found = true;
        }
        else
        {
            i++;
        }
    }

    return found;
} */

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

QVector<GraphEdge> Graph::GetIncidentEdges(GraphNode* node)
{
    QVector<GraphEdge> ret;

    for (int i = 0; i < edges.length(); i++)
    {
        if (edges[i].node1 == node || edges[i].node2 == node)
        {
            ret.push_back(edges[i]);
        }
    }

    return ret;
}

GraphNode& Graph::GetNodeByID(int id)
{
    for (int i = 0; i < nodes.length(); i++)
    {
        if (nodes[i].id == id)
        {
            return nodes[i];
        }
    }

    return nodes[0]; // исправить
}

void Graph::printNodes()
{
    for (int i = 0; i < nodes.length(); i++)
    {
        qDebug() << nodes[i].id << nodes[i].x << nodes[i].y;
    }
}
