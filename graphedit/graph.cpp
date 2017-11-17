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

bool Graph::AddEdge(int a, int b, int w)
{
    qDebug() << "Graph::AddEdge(" << a << ", " << b << ", " << w << ")";

    if (a == b) return false;

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

    if (!found && GetNodeIndexByID(a) != -1 && GetNodeIndexByID(b) != -1)
    {
        GraphEdge ed(a, b, w, false);

        edges.append(ed);

        return true;
    }
    else
    {
        return false;
    }
}

bool Graph::RemoveNode(int id)
{
    int j = GetNodeIndexByID(id);
    if (j != -1)
    {
        nodes.remove(j);
    }
    else
    {
        return false;
    }

    int i = 0;
    while (i < edges.length())
    {
        if (edges[i].node1 == id || edges[i].node2 == id)
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

bool Graph::RemoveEdge(int a, int b)
{
    bool found = false ;
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

void Graph::printNodes()
{
    for (int i = 0; i < nodes.length(); i++)
    {
        qDebug() << nodes[i].id << nodes[i].x << nodes[i].y;
    }
}
