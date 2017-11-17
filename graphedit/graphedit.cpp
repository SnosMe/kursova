#include <algorithm>
#include <cmath>
#include <QVariant>
#include <QDebug>
#include <QMouseEvent>
#include "graphedit.h"

const float LINE_WIDTH = 0.05;

static int dist(int x, int y)
{
    return (int) sqrt(x*x+y*y);
}

GraphEdit::GraphEdit(QWidget *parent) : QWidget(parent)
{
    selected = -1;
    mode     = MODE_NONE;
    setFocusPolicy(Qt::StrongFocus);
}

void GraphEdit::paintEvent(QPaintEvent*)
{
    g.printNodes();

    QPainter p(this);

    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    r = std::min(w, h)*0.1;

    for (int i = 0; i < g.edges.length(); i++)
    {
        int nd1 = g.edges[i].node1;
        int nd2 = g.edges[i].node2;

        int j1 = g.GetNodeIndexByID(nd1);
        int j2 = g.GetNodeIndexByID(nd2);

        int x1 = g.nodes[j1].x*w;
        int y1 = g.nodes[j1].y*h;
        int x2 = g.nodes[j2].x*w;
        int y2 = g.nodes[j2].y*h;

        if (i == selected && mode == MODE_SELEDGE)
        {
            p.setPen(QPen(QColor(255, 0, 0)));
        }
        else if (g.edges[i].colored)
        {
            p.setPen(QPen(QColor(0, 255, 0)));
        }
        else
        {
            p.setPen(QPen(QColor(0, 0, 0)));
        }

        p.drawLine(x1, y1, x2, y2);

        p.drawText((x1+x2)/2, (y1+y2)/2, QVariant(g.edges[i].w).toString());
    }

    p.setPen(QPen(QColor(0, 0, 0)));

    if (mode == MODE_NEWEDGE)
    {
        p.drawLine(from, to);
    }

    for (int i = 0; i < g.nodes.length(); i++)
    {
            if (g.nodes[i].id == selected && mode == MODE_MOVE)
            {
                p.setBrush(QBrush(QColor(190, 190, 255)));
            }
            else if (g.nodes[i].colored)
            {
                p.setBrush(QBrush(QColor(0, 255, 0)));
            }
            else
            {
                p.setBrush(QBrush(QColor(255, 255, 255)));
            }

            int x = g.nodes[i].x*w - r/2;
            int y = g.nodes[i].y*h - r/2;
            p.drawEllipse(x, y, r, r);

            p.drawText(QRectF(x, y, r, r), Qt::AlignCenter, QVariant(g.nodes[i].id).toString());
    }
}

void GraphEdit::mousePressEvent(QMouseEvent* e)
{
    int x = e->x();
    int y = e->y();

    int id = getNodeAt(x, y);

    if (id != -1)
    {
        selected = id;
        from.setX(x);
        from.setY(y);
        to.setX(x);
        to.setY(y);

        if (mode == MODE_NONE)
        {
            mode = MODE_NEWEDGE;
            qDebug() << "MODE_NEWEDGE";
        }
        else if (mode == MODE_SELEDGE)
        {
            emit edgeSelectionLoss();
            mode = MODE_NEWEDGE;
            qDebug() << "MODE_NEWEDGE";
        }

    }
    else
    {
        id = getEdgeAt(x, y);

        qDebug() << "id = " << id;

        if (id != -1)
        {
            selected = id;
            mode = MODE_SELEDGE;
            emit edgeSelected(id);
            qDebug() << "MODE_SELEDGE";
        }
        else
        {
            selected = -1;
            if (mode == MODE_SELEDGE) emit edgeSelectionLoss();
            mode = MODE_NONE;
            qDebug() << "MODE_NONE";
        }
    }

    update();
}

void GraphEdit::mouseReleaseEvent(QMouseEvent* e)
{
    if (mode == MODE_NEWEDGE)
    {
        int id = getNodeAt(e->x(), e->y());
        if (id != -1)
        {
            if (id != selected && from != to)
            {
                g.AddEdge(selected, id, 1);
            }
        }

        mode = MODE_NONE;
        qDebug() << "MODE_NONE";
        update();
    }
}

void GraphEdit::mouseMoveEvent(QMouseEvent* e)
{
    if (mode == MODE_NEWEDGE)
    {
        to.setX(e->x());
        to.setY(e->y());

        update();
    }
    else if (mode == MODE_MOVE)
    {
        if (selected != -1)
        {
            float w, h;
            QSize sz = this->size();

            w = sz.width();
            h = sz.height();
            int x = e->x();
            int y = e->y();



            if (x >= 0 && x <= w && y >= 0 && y <= w)
            {
                int index = g.GetNodeIndexByID(selected);

                g.nodes[index].x = e->x()/w;
                g.nodes[index].y = e->y()/h;

                update();
            }
        }
    }
}

void GraphEdit::keyReleaseEvent(QKeyEvent* e)
{
    e->accept();

    if (e->key() == Qt::Key_Delete)
    {
        if (selected != -1)
        {
            if (mode == MODE_MOVE)
            {
                g.RemoveNode(selected);
                selected = -1;
                mode = MODE_NONE;
            }
            else if(mode == MODE_SELEDGE)
            {
                g.edges.remove(selected);
                emit edgeSelectionLoss();
                selected = -1;
                mode = MODE_NONE;
            }
        }
        update();
    }
}

void GraphEdit::mouseDoubleClickEvent(QMouseEvent* e)
{
    int x = e->x();
    int y = e->y();

    int id = getNodeAt(x, y);

    if (id != -1)
    {
        selected = id;

        if (mode == MODE_SELEDGE) emit edgeSelectionLoss();
        mode = MODE_MOVE;

        qDebug() << "MODE_MOVE";
    }
    else
    {
        selected = -1;
        mode = MODE_NONE;
        qDebug() << "MODE_NONE";
    }

    update();
}



int GraphEdit::GetSelectedEdge()
{
    if (mode == MODE_SELEDGE) return selected;
    else return -1;
}

void GraphEdit::SetEdgeWeight(int id, int w)
{
    if (id >= 0 && id < g.edges.length())
    {
        g.edges[id].w = w;
    }
}

/*
int** GraphEdit::GetMatrix()
{
    int** ret = new int*[nodes.length()];

    for (int i = 0; i < nodes.length(); i++)
    {
        ret[i] = new int[nodes.length()];

        for (int j = 0; j < nodes.length(); j++)
        {
            ret[i][j] = 0;
        }
    }

    for (int i = 0; i < edges.length(); i++)
    {
        int v1 = edges[i].node1;
        int v2 = edges[i].node2;

        ret[v1][v2] = edges[i].w;
        ret[v2][v1] = edges[i].w;
    }

    return ret;
}

void GraphEdit::HighlightEdge(int a, int b, bool v)
{
    for (int i = 0; i < edges.length(); i++)
    {
        if ((edges[i].node1 == a &&
             edges[i].node2 == b) ||
                (edges[i].node1 == b &&
                 edges[i].node2 == a))
        {
            hl_edge[i] = v;
            break;
        }
    }

    update();
}

void GraphEdit::HighlightNode(int i, bool v)
{
    if (i >= 0 && i < nodes.length()) hl_node[i] = v;
}

int GraphEdit::GetSize()
{
    return nodes.length();
}
*/

int GraphEdit::getNodeAt(int x, int y)
{
    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    r = std::min(w, h)*0.1;

    for (int i = 0; i < g.nodes.length(); i++)
    {
        int d = dist(g.nodes[i].x*w - x, g.nodes[i].y*h - y);
        if (d <= r)
        {
            return g.nodes[i].id;
        }
    }

    return -1;
}

int GraphEdit::getEdgeAt(int x, int y)
{

    float w, h;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();

    float rx = x/w;
    float ry = y/h;

    for (int i = 0; i < g.edges.length(); i++)
    {
        int v1 = g.edges[i].node1;
        int v2 = g.edges[i].node2;

        int i1 = g.GetNodeIndexByID(v1);
        int i2 = g.GetNodeIndexByID(v2);

        float x1 = g.nodes[i1].x;
        float y1 = g.nodes[i1].y;
        float x2 = g.nodes[i2].x;
        float y2 = g.nodes[i2].y;

        qDebug() << x1 << rx << x2 << y1 << ry << y2;

        if (rx <= std::max(x1, x2)+LINE_WIDTH && rx >= std::min(x1, x2)-LINE_WIDTH &&
            ry <= std::max(y1, y2)+LINE_WIDTH && ry >= std::min(y1, y2)-LINE_WIDTH)
        {
            float a = fabs((y2-y1)*rx-(x2-x1)*ry+x2*y1-y2*x1);
            float b = sqrt(pow(x2-x1, 2)+pow(y2-y1, 2));
            float d = a/b;

            qDebug() << "d = " << d;

            if (d <= LINE_WIDTH) return i;
        }
    }

    return -1;
}

bool GraphEdit::AddNode()
{
    g.AddNode();
    update();
}

Graph GraphEdit::GetGraph()
{
    return g;
}

void GraphEdit::SetGraph(Graph src)
{
    g.nodes = src.nodes;
    g.edges = src.edges;
    update();
}

/*QVector<GraphEdge> GraphEdit::GetEdges()
{
    return edges;
}

QVector<GraphNode> GraphEdit::GetNodes()
{
    return nodes;
}*/
