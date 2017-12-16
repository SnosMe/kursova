#ifndef GRAPHNODE
#define GRAPHNODE

#include <QVector>
#include <QList>
#include <QDebug>

enum struct ColorMode
{
    CLEAR = 0,
    GREEN,
    BOLD
};

struct GraphNode
{
    float x;         // rendering
    float y;         // rendering
    int id;          // internal unique id
    QString name;    // node friendly name
    ColorMode color; // rendering

    bool existInTrees = false;
    int length = 0;

    GraphNode(int id = 0, float x = 0.5, float y = 0.5, ColorMode color = ColorMode::CLEAR) :
        x(x), y(y), id(id), name(QString::number(id)), color(color) {}
};

struct GraphEdge
{
    GraphNode* node1;
    GraphNode* node2;
    int w;        // also known as capacity
    int flow = 0; // flow <= capacity
    ColorMode color;

    GraphEdge(GraphNode* node1 = 0, GraphNode* node2 = 0, int weight = 1, ColorMode color = ColorMode::CLEAR):
        node1(node1), node2(node2), w(weight), color(color) {}

    static bool lessThan(const GraphEdge* e1, const GraphEdge* e2);
};

struct Graph
{
    QList<GraphNode> nodes;
    QVector<GraphEdge> edges;
    GraphNode* begin = nullptr;
    GraphNode* end = nullptr;
    bool directed = false;

    bool AddNode(float x = 0.5, float y = 0.5, int id = 0);
    bool AddEdge(GraphNode* a, GraphNode* b, int w);
    bool RemoveNode(GraphNode* node);
    bool RemoveEdge(GraphEdge* edge);
    int GetNodeIndexByID(int);
    GraphEdge* getEdgeByNodePair(GraphNode* node1, GraphNode* node2);
    QVector<GraphEdge*> GetIncidentEdges(GraphNode* node);
    QVector<GraphEdge*> getLowestEdge(QVector<GraphEdge*> edges);
    QVector<GraphEdge*> getLowestEdge();
    GraphNode* GetNodeByID(int);
    void setBeginNode(GraphNode* node);
    void setEndNode(GraphNode* node);
    void colorGreenToBold();

    Graph() {}
    Graph(const Graph& src) { *this = src; }
    Graph& operator=(const Graph& src);
};

#endif // GRAPHNODE
