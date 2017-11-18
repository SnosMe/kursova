#ifndef GRAPHNODE
#define GRAPHNODE

#include <QVector>

struct GraphNode
{
    float x;      // rendering
    float y;      // rendering
    int id;       // node friendly name
    bool colored; // rendering

    bool existInTrees; // dijkstra
    int length;        // dijkstra

    GraphNode(int id = 0, float x = 0.5, float y = 0.5, bool colored = false)
    {
        this->x  = x;
        this->y  = y;
        this->id = id;
        this->colored = colored;

        existInTrees = false;
        length = 0;
    }
};

struct GraphEdge
{
    GraphNode* node1;
    GraphNode* node2;
    // int node1;
    // int node2;
    int w;
    bool colored;

    GraphEdge(GraphNode* node1 = 0, GraphNode* node2 = 0, int weight = 1, bool colored = false)
    {
        this->node1 = node1;
        this->node2 = node2;
        this->w = weight;
        this->colored = colored;
    }

    ~GraphEdge ()
    {
        // delete n1; - managed by QVector
        // delete n2; - managed by QVector
    }
};

struct Graph
{
    QVector<GraphNode> nodes;
    QVector<GraphEdge> edges;

    bool AddNode(float x = 0.5, float y = 0.5, int id = 0);
    bool AddEdge(GraphNode* a, GraphNode* b, int w);
    bool RemoveNode(GraphNode* node);
    // bool RemoveEdge(int, int);
    int  GetNodeIndexByID(int);
    QVector<GraphEdge> GetIncidentEdges(GraphNode* node);
    GraphNode& GetNodeByID(int);

    void printNodes();

   /* Graph(const Graph& src)
    {
        nodes = src.nodes;
        edges = src.edges;
    }*/
};

#endif // GRAPHNODE

