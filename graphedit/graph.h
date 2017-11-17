#ifndef GRAPHNODE
#define GRAPHNODE

#include <QVector>

struct GraphNode
{
    float x;
    float y;
    int   id;
    bool  colored;

    GraphNode()
    {
        x  = 0.5;
        y  = 0.5;
        id = 0;
        colored = false;
    }

    GraphNode(int i, float px, float py, bool c)
    {
        x  = px;
        y  = py;
        id = i;
        colored = c;
    }
};

struct GraphEdge
{
    int  node1;
    int  node2;
    int  w;
    bool colored;

    GraphEdge()
    {
        node1 = 0;
        node2 = 0;
        w = 1;
        colored = false;
    }

    GraphEdge(int n1, int n2, int wgh, bool c)
    {
        node1 = n1;
        node2 = n2;
        w = wgh;
        colored = c;
    }
};

struct Graph
{
    QVector<GraphNode> nodes;
    QVector<GraphEdge> edges;

    bool AddNode(float x = 0.5, float y = 0.5, int id = 0);
    bool AddEdge(int a, int b, int w);
    bool RemoveNode(int);
    bool RemoveEdge(int, int);
    int  GetNodeIndexByID(int);
    QVector<GraphEdge> GetIncidentEdges(int);
    GraphNode& GetNodeByID(int);

    void printNodes();

   /* Graph(const Graph& src)
    {
        nodes = src.nodes;
        edges = src.edges;
    }*/
};

#endif // GRAPHNODE

