#ifndef GRAPHEDIT_H
#define GRAPHEDIT_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include "graph.h"

class GraphEdit : public QWidget
{
    Q_OBJECT
public:
    explicit GraphEdit(QWidget *parent = 0);
    int  AddNode(float x, float y, int w);
    int  AddNode(int w);
    int  AddNode();
    int  AddNode(int x, int y);
    void AddEdge(int a, int b, int w);
    bool RemoveNode(int);
    bool RemoveEdge(int, int);
    int  GetSelected(int);
    QVector< QVector<int> > GetMatrix();

signals:
    //void nodeSelected(int);
    void twoNodesSelected(int, int);
   // void selectionLoss();
    void dblSelectionLoss();

//public slots:

private:
    QVector<GraphNode> nodes;
    QVector<GraphEdge> edges;
    QVector<int>       removed;
    int selected[2];
    bool multi_select;
    bool grab;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
};

#endif // GRAPHEDIT_H
