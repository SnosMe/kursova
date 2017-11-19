#ifndef GRAPHEDIT_H
#define GRAPHEDIT_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include "graph.h"

enum EditorMode
{
    MODE_NONE,
    MODE_NEWEDGE,
    MODE_MOVE,
    MODE_SELEDGE
};

class GraphEdit : public QWidget
{
    Q_OBJECT
public:
    explicit GraphEdit(QWidget *parent = 0);

    GraphEdge* GetSelectedEdge(); // возвращает индекс выделеного ребра, или -1 если ребро не выделено
    void SetEdgeWeight(GraphEdge*, int);  // установить вес ребра
   /* int** GetMatrix(); // возвращает матрицу смежности
    void HighlightEdge(int a, int b, bool v = true); // подсветить ребро
    void HighlightNode(int i, bool v = true); // подсветить вершину
    int GetSize(); // возвращает количество вершин*/
    bool AddNode(int x, int y);
    GraphNode*          selectedNode;
    GraphEdge*          selectedEdge;

    Graph& GetGraph();
    void SetGraph(Graph);
    void clearInternalState();

signals:
    void edgeSelected(GraphEdge* edge);
    void edgeSelectionLoss();
    void nodeSelected(GraphNode* node);
    void nodeSelectionLoss();

private:
    Graph g;
   /* QVector<GraphNode>  nodes;
    QVector<GraphEdge>  edges;
    QVector<bool>       removed;
    QVector<bool>       hl_edge;
    QVector<bool>       hl_node;*/
    EditorMode          mode;
    QPoint              from;
    QPoint              to;

protected:
    GraphNode* getNodeAt(int x, int y);
    GraphEdge* getEdgeAt(int x, int y);
    //int getNodeIndexByID(int);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    //void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
};

#endif // GRAPHEDIT_H
