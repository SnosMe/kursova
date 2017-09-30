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

        p.drawText((x1+x2)/2, (y1+y2)/2, QVariant(edges[i].w).toString());
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

            p.drawText(QRectF(x, y, r, r), Qt::AlignCenter, QVariant(nodes[i].w).toString());
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
                if (selected[1] != -1) emit dblSelectionLoss();
                selected[1] = -1;
                // emit nodeSelected(i);
                found = true;
                break;
            }
            else
            {
                std::swap(selected[0], selected[1]);
                selected[0] = i;

                if (selected[1] != -1) emit twoNodesSelected(selected[0], selected[1]);
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        if (selected[0] != -1 && selected[1] != -1) emit dblSelectionLoss();

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
    e->accept();

    if (e->key() == Qt::Key_Control)
    {
        multi_select = false;
    }
    else if (e->key() == Qt::Key_Delete)
    {
        if (selected[0] != -1)
        {
            if (selected[1] == -1)
            {
                RemoveNode(selected[0]);
                selected[0] = -1;
            }
            else
            {
                RemoveEdge(selected[0], selected[1]);
            }
        }
        update();
    }
    else if (e->key() == Qt::Key_Insert)
    {
        if (selected[0] != -1 && selected[1] != -1)
        {
            AddEdge(selected[0], selected[1], 1);
        }
        update();
    }
}

int GraphEdit::AddNode(float x, float y, int w)
{
    GraphNode nd;
    nd.x = x;
    nd.y = y;
    nd.w = w;

    removed.append(false);
    nodes.append(nd);

    update();

    return 1;
}

int GraphEdit::AddNode(int w)
{
    AddNode(0.5, 0.5, w);

    return 1;
}

int GraphEdit::AddNode()
{
    AddNode(0.5, 0.5, nodes.length()+1);

    return 1;
}

int GraphEdit::AddNode(int x, int y)
{
    AddNode(x, y, nodes.length()+1);

    return 1;
}

void GraphEdit::AddEdge(int a, int b, int w)
{
    qDebug() << "AddEdge" << a << " " << b << " " << w;
    if (a != b)
    {
        GraphEdge ed;
        ed.node1 = a;
        ed.node2 = b;
        ed.w     = w;

        edges.append(ed);
        update();
    }
}

bool GraphEdit::RemoveNode(int id)
{
    if (id < removed.length())
    {
        removed[id] = true;

        int i = 0;
        while (i < edges.length())
        {
            //qDebug() << "1: " << edges[i].node1 << "\t" << edges[i].node2;

            if (edges[i].node1 == id ||
                    edges[i].node2 == id)
            {
                //qDebug() << "removed";
                edges.remove(i);
            }
            else
            {
                i++;
            }
        }

        update();
        return true;
    }
    else return false;
}

bool GraphEdit::RemoveEdge(int a, int b)
{
    bool found = false ;
    int i = 0;

    while (i < edges.length())
    {
        if ((edges[i].node1 == a &&
             edges[i].node2 == b) ||
                (edges[i].node1 == b &&
                 edges[i].node2 == a))
        {
            edges.remove(i);
            found = true;
        }
        else
        {
            i++;
        }
    }

    update();

    return found;
}

int GraphEdit::GetSelected(int i)
{
    if (i == 0 || i == 1)
    {
        return selected[i];
    }
    else
    {
        return -1;
    }
}
