#include <algorithm>
#include <cmath>
#include <QVariant>
#include <QDebug>
#include <QMouseEvent>
#include "graphedit.h"

static int dist(int x, int y)
{
    return (int) sqrt(x*x+y*y);
}

GraphEdit::GraphEdit(QWidget *parent) : QWidget(parent)
{
    selected[0] = -1;
    selected[1] = -1;
    multi_select = false;
    grab = false;
    setFocusPolicy(Qt::StrongFocus);
}

void GraphEdit::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    r = std::min(w, h)*0.1;

    for (int i = 0; i < edges.length(); i++)
    {
        int nd1 = edges[i].node1;
        int nd2 = edges[i].node2;

        int x1 = nodes[nd1].x*w;
        int y1 = nodes[nd1].y*h;
        int x2 = nodes[nd2].x*w;
        int y2 = nodes[nd2].y*h;

        p.drawLine(x1, y1, x2, y2);

    }

    for (int i = 0; i < nodes.length(); i++)
    {
        if (!removed[i])
        {
            if (i == selected[0] || i == selected[1])
            {
                p.setBrush(QBrush(QColor(190, 190, 255)));
            }
            else
            {
                p.setBrush(QBrush(QColor(255, 255, 255)));
            }

            int x = nodes[i].x*w - r/2;
            int y = nodes[i].y*h - r/2;
            p.drawEllipse(x, y, r, r);

            p.drawText(QRectF(x, y, r, r), Qt::AlignCenter, QVariant(nodes[i].weight).toString());
        }
    }
}

void GraphEdit::mousePressEvent(QMouseEvent* event)
{
    int x, y;

    x = event->x();
    y = event->y();

    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    r = std::min(w, h)*0.1;

    bool found = false;

    for (int i = 0; i < nodes.length(); i++)
    {
        int d = dist(nodes[i].x*w - x, nodes[i].y*h - y);
        if (d <= r)
        {
            if (!multi_select)
            {
                selected[0] = i;
                selected[1] = -1;
                found = true;
                break;
            }
            else
            {
                std::swap(selected[0], selected[1]);
                selected[0] = i;
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        selected[0] = -1;
        selected[1] = -1;
    }

    grab = true;

    update();
}

void GraphEdit::mouseReleaseEvent(QMouseEvent* e)
{
   grab = false;

  // update();
}

void GraphEdit::mouseMoveEvent(QMouseEvent* e)
{
    if (grab && selected[0] != -1)
    {
        float w, h;
        QSize sz = this->size();

        w = sz.width();
        h = sz.height();

        float x = e->x()/w;
        float y = e->y()/h;

        int id = selected[0];

        if (x >= 0.0 && x <= 1.0 &&
            y >= 0.0 && y <= 1.0)
        {
            nodes[id].x = x;
            nodes[id].y = y;

            update();
        }
    }
}

void GraphEdit::keyPressEvent(QKeyEvent* e)
{
    //qDebug() << "keyPressEvent";

    e->accept();

    switch (e->key())
    {
        case Qt::Key_Control:
            multi_select = true;
        break;
    }
}

void GraphEdit::keyReleaseEvent(QKeyEvent* e)
{
    //qDebug() << "keyReleaseEvent";
    e->accept();

    switch (e->key())
    {
    case Qt::Key_Control:
        multi_select = false;
        break;
    case Qt::Key_Delete:
        deleteKey();
        update();
   /* case Qt::Key_Insert:
        insertKey();
        update();
        break;*/
    }
}

int GraphEdit::AddNode(float x, float y, int w)
{
    GraphNode nd;
    nd.x = x;
    nd.y = y;
    nd.weight = w;

    removed.append(false);
    nodes.append(nd);

    return 1;
}

int GraphEdit::AddNode(int w)
{
    AddNode(0.5, 0.5, w);

    return 1;
}

void GraphEdit::AddEdge(int a, int b)
{
    GraphEdge ed;
    ed.node1 = a;
    ed.node2 = b;

    edges.append(ed);
}

void GraphEdit::deleteKey()
{
    if (selected[0] != -1)
    {
        if (selected[1] == -1)
        {
            removed[selected[0]] = true;

            for (int i = 0; i < edges.length(); i++)
            {
                if (edges[i].node1 == selected[0] ||
                    edges[i].node2 == selected[0])
                {
                    qDebug() << "1: " << edges[i].node1 << "\t" << edges[i].node2;
                    edges.remove(i);
                }
            }

            selected[0] = -1;
        }
        else
        {
            for (int i = 0; i < edges.length(); i++)
            {
                if ((edges[i].node1 == selected[0] &&
                    edges[i].node2 == selected[1]) ||
                    (edges[i].node1 == selected[1] &&
                    edges[i].node2 == selected[0]))
                {
                    qDebug() << "2: " << edges[i].node1 << "\t" << edges[i].node2;
                    edges.remove(i);
                }
            }
        }
    }
}

/*void GraphEdit::insertKey()
{
    if (selected[0] != -1 && selected[1] != -1)
    {

    }
}*/


