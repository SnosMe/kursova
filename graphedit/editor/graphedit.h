#ifndef GRAPHEDIT_H
#define GRAPHEDIT_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include "../graph.h"

enum EditorMode
{
    MODE_NONE,
    MODE_NEWEDGE,
    MODE_MOVE,
    MODE_SELEDGE,
    MODE_MOVE_VP
};

class GraphEdit : public QWidget
{
    Q_OBJECT
public:
    explicit GraphEdit(QWidget *parent = nullptr);

    bool AddNode(int x, int y);
    GraphNode*          selectedNode;
    GraphEdge*          selectedEdge;

    Graph& GetGraph();
    void SetGraph(const Graph &);
    void clearInternalState();

public slots:
    void setSelectedObjectText(const QString& str);
    void setIsDirectedGraph(int state);

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
    QRectF              viewport;
    QPointF             old_vp_pos;
    void openNodeDialog(GraphNode* node);
    void openEdgeDialog(GraphEdge* edge);

protected:
    GraphNode* getNodeAt(int x, int y);
    GraphEdge* getEdgeAt(int x, int y);
    QPointF    toScreenCoords(QPointF);
    QPointF    toInternalCoords(QPointF);
    //int getNodeIndexByID(int);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    //void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void wheelEvent(QWheelEvent*);
};

#endif // GRAPHEDIT_H
