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
    float x;      // rendering
    float y;      // rendering
    int id;       // internal unique id
    QString name; // node friendly name
    ColorMode color; // rendering

    bool existInTrees; // dijkstra
    int length;        // dijkstra

    GraphNode(int id = 0, float x = 0.5, float y = 0.5, ColorMode color = ColorMode::CLEAR)
    {
        this->x  = x;
        this->y  = y;
        this->id = id;
        this->name = QString::number(id);
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
    QList<GraphNode> nodes;
    QVector<GraphEdge> edges;
    GraphNode* begin;
    GraphNode* end;
    bool directed;

    bool AddNode(float x = 0.5, float y = 0.5, int id = 0);
    bool AddEdge(GraphNode* a, GraphNode* b, int w);
    bool RemoveNode(GraphNode* node);
    bool RemoveEdge(GraphEdge* edge);
    int  GetNodeIndexByID(int);
    QVector<GraphEdge*> GetIncidentEdges(GraphNode* node);
    QVector<GraphEdge*> getLowestEdge(QVector<GraphEdge*> edges);
    GraphNode* GetNodeByID(int);
    void setBeginNode(GraphNode* node);
    void setEndNode(GraphNode* node);

    void printNodes();

    Graph()
    {
        begin = nullptr;
        end = nullptr;
        directed = false;
    }

    Graph(const Graph& src)
    {
        qDebug() << "Graph::CopyCtor()";
        *this = src;
    }

    Graph& operator=(const Graph& src)
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
};

#endif // GRAPHNODE
