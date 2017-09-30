#ifndef GRAPHNODE
#define GRAPHNODE

struct GraphNode
{
    float  x;
    float  y;
    float  weight;
};

struct GraphEdge
{
    int node1;
    int node2;
};

#endif // GRAPHNODE

