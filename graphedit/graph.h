#ifndef GRAPHNODE
#define GRAPHNODE

struct GraphNode
{
    float x;
    float y;
    int   w;
};

struct GraphEdge
{
    int node1;
    int node2;
    int w;
};

#endif // GRAPHNODE

