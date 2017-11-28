#include "fordfulkerson.h"

FordFulkerson::FordFulkerson(QList<Graph> *states)
{
    this->states = states;
    gCopy = (*states)[0];
    n = gCopy.nodes.size();
    head = tail = 0;

    q = new int[n+2];
    pred = new int[n];
    color = new int[n];
    capacity = new int*[n];
    flow = new int*[n];

    for (int i = 0; i<n; i++)
    {
        capacity[i] = new int[n];
        flow[i] = new int[n];

        for (int j = 0; j<n; j++) {
            capacity[i][j] = 0;
            flow[i][j] = 0;
        }
    }

    for (GraphEdge& edge : gCopy.edges)
    {
        capacity[getNodeIdx(edge.node1)][getNodeIdx(edge.node2)] = edge.w;
    }
    qDebug() << "Ford:" << maxFlow(getNodeIdx(gCopy.begin), getNodeIdx(gCopy.end));
}

FordFulkerson::~FordFulkerson()
{
    delete[] q;
    delete[] pred;
    delete[] color;

    for (int i = 0; i<n; i++)
    {
        delete[] capacity[i];
        delete[] flow[i];
    }

    delete[] capacity;
    delete[] flow;
}

void FordFulkerson::enqueue(int x)
{
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

int FordFulkerson::dequeue()
{
    int x = q[head];
    head++;
    color[x] = BLACK;
    return x;
}

int FordFulkerson::bfs(int start, int target)
{
    int u, v;
    for (u = 0; u<n; u++) {
        color[u] = WHITE;
    }
    head = tail = 0;
    enqueue(start);
    pred[start] = -1;
    while (head != tail)
    {
        u = dequeue();
        for (v = 0; v<n; v++)
        {
            if (color[v] == WHITE && capacity[u][v] - flow[u][v]>0)
            {
                enqueue(v);
                pred[v] = u;
            }
        }
    }
    return color[target] == BLACK;
}

int FordFulkerson::maxFlow(int source, int sink)
{
    int maxFlow = 0;
    int cntZero = 0;

    while (bfs(source, sink))
    {
        int increment = INT_MAX;
        for (int u = n - 1; pred[u] >= 0; u = pred[u]) {
            increment = qMin(increment, capacity[pred[u]][u] - flow[pred[u]][u]);
        }
        for (int u = n - 1; pred[u] >= 0; u = pred[u]) {
            flow[pred[u]][u] += increment;
            flow[u][pred[u]] -= increment;
        }
        maxFlow += increment;

        if (increment == 0) cntZero++;
        if (cntZero == 10) break;
    }
    return maxFlow;
}

int FordFulkerson::getNodeIdx(GraphNode *node)
{
    for (int i = 0; i < gCopy.nodes.size(); i++)
    {
        if (&gCopy.nodes[i] == node)
        {
            return i;
        }
    }
    return -1;
}
