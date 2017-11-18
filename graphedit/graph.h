#ifndef GRAPHNODE
#define GRAPHNODE

#include <QVector>

enum struct ColorMode
{
    CLEAR = 0,
    GREEN,
    BOLD
};

struct GraphNode
{
    float x;      // rendering
    float y;      // rendering
    int id;       // node friendly name
    ColorMode color; // rendering

    bool existInTrees; // dijkstra
    int length;        // dijkstra

    GraphNode(int id = 0, float x = 0.5, float y = 0.5, ColorMode color = ColorMode::CLEAR)
    {
        this->x  = x;
        this->y  = y;
        this->id = id;
        this->color = color;

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
    ColorMode color;

    GraphEdge(GraphNode* node1 = 0, GraphNode* node2 = 0, int weight = 1, ColorMode color = ColorMode::CLEAR)
    {
        this->node1 = node1;
        this->node2 = node2;
        this->w = weight;
        this->color = color;
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
    QVector<GraphEdge*> GetIncidentEdges(GraphNode* node);
    QVector<GraphEdge*> getLowestEdge(QVector<GraphEdge*> edges);
    GraphNode* GetNodeByID(int);

    void printNodes();

    Graph() {}

    Graph(const Graph& src)
    {
        nodes = src.nodes;
        edges = src.edges;
        for (auto& edge : edges) {
            edge.node1 = GetNodeByID(edge.node1->id);
            edge.node2 = GetNodeByID(edge.node2->id);
        }
    }
};

#endif // GRAPHNODE

