#include <algorithm>
#include <cmath>
#include <QVariant>
#include <QDebug>
#include <QMouseEvent>
#include "mainwindow.h"
#include "graphedit.h"

const float LINE_WIDTH = 0.05;

static int dist(int x, int y)
{
    return (int) sqrt(x*x+y*y);
}

GraphEdit::GraphEdit(QWidget *parent) : QWidget(parent)
{
    mode = MODE_NONE;
    selectedEdge = nullptr;
    selectedNode = nullptr;
    setFocusPolicy(Qt::StrongFocus);
}

void GraphEdit::paintEvent(QPaintEvent*)
{
    g.printNodes();

    QPainter p(this);

    p.setRenderHints(QPainter::Antialiasing);
    p.setFont(QFont("Consolas", 10));

    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    r = std::min(w, h)*0.1;

    for (int i = 0; i < g.edges.length(); i++)
    {
        int x1 = g.edges[i].node1->x*w;
        int y1 = g.edges[i].node1->y*h;
        int x2 = g.edges[i].node2->x*w;
        int y2 = g.edges[i].node2->y*h;

        if (&g.edges[i] == selectedEdge && mode == MODE_SELEDGE)
        {
            p.setPen(QPen(QColor(255, 0, 0), 1.0));
        }
        else if (g.edges[i].color == ColorMode::GREEN)
        {
            p.setPen(QPen(QColor(0, 200, 83), 3.0));
        }
        else if (g.edges[i].color == ColorMode::BOLD)
        {
            p.setPen(QPen(QColor(33, 33, 33), 3.0));
        }
        else
        {
            p.setPen(QPen(QColor(33, 33, 33), 1.0));
        }

        p.drawLine(x1, y1, x2, y2);

        p.setPen(QPen(QColor(33, 33, 33), 1.0));
        p.drawText((x1+x2)/2, (y1+y2)/2, QVariant(g.edges[i].w).toString());
    }

    p.setPen(QPen(QColor(33, 33, 33)));

    if (mode == MODE_NEWEDGE)
    {
        p.drawLine(from, to);
    }


    for (int i = 0; i < g.nodes.length(); i++)
    {
        QPen textPan = QPen(QColor(33, 33, 33), 1.0);

        if (&g.nodes[i] == selectedNode && mode == MODE_MOVE)
        {
            textPan = QPen(QColor(245, 245, 245), 1.0);
            p.setBrush(QBrush(QColor(33, 33, 33)));
        }
        else if (g.nodes[i].color == ColorMode::GREEN)
        {
            p.setPen(QPen(QColor(0, 200, 83), 1.0)); // border color
            p.setBrush(QBrush(QColor(0, 200, 83))); // inset color
        }
        else if (g.nodes[i].color == ColorMode::BOLD)
        {
            p.setPen(QPen(QColor(33, 33, 33), 3.0));
            p.setBrush(QBrush(QColor(255, 255, 255)));
        }
        else
        {
            p.setPen(QPen(QColor(33, 33, 33), 1.0));
            p.setBrush(QBrush(QColor(255, 255, 255)));
        }

        int x = g.nodes[i].x*w - r/2;
        int y = g.nodes[i].y*h - r/2;
        p.drawEllipse(x, y, r, r);

        p.setPen(textPan);
        p.drawText(QRectF(x, y, r, r), Qt::AlignCenter, g.nodes[i].name);
    }
}

void GraphEdit::mousePressEvent(QMouseEvent* e)
{
    int x = e->x();
    int y = e->y();

    GraphNode* node = getNodeAt(x, y);

    if (node != nullptr)
    {
        selectedNode = node;
        selectedEdge = nullptr;
        from.setX(x);
        from.setY(y);
        to.setX(x);
        to.setY(y);

        if (mode == MODE_MOVE)
        {
            emit nodeSelected(node);
        }

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
        update();
    }
    else
    {
        GraphEdge* edge = getEdgeAt(x, y);

        if (edge != nullptr)
        {
            selectedEdge = edge;
            selectedNode = nullptr;
            if (mode == MODE_MOVE) emit nodeSelectionLoss();
            mode = MODE_SELEDGE;
            emit edgeSelected(edge);
            update();
            qDebug() << "MODE_SELEDGE";
        }
        else
        {
            selectedEdge = nullptr;
            selectedNode = nullptr;
            if (mode == MODE_SELEDGE) {
                update();
                emit edgeSelectionLoss();
            }
            else if (mode == MODE_MOVE)
            {
                update();
                emit nodeSelectionLoss();
            }
            mode = MODE_NONE;
            qDebug() << "MODE_NONE";
        }
    }
}

void GraphEdit::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (mode == MODE_NEWEDGE)
        {
            GraphNode* node = getNodeAt(e->x(), e->y());
            if (node != nullptr)
            {
                if (node != selectedNode && from != to)
                {
                    g.AddEdge(selectedNode, node, 1);
                }
            }

            mode = MODE_NONE;
            qDebug() << "MODE_NONE";
            update();
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        if (selectedEdge != nullptr ||
            selectedNode != nullptr)
        {
            if (mode == MODE_MOVE)
            {
                g.RemoveNode(selectedNode);
                selectedNode = nullptr;
                emit nodeSelectionLoss();
                mode = MODE_NONE;
            }
            else if (mode == MODE_SELEDGE)
            {
                g.RemoveEdge(selectedEdge);
                emit edgeSelectionLoss();
                selectedEdge = nullptr;
                mode = MODE_NONE;
            } else {
                int x = e->x();
                int y = e->y();

                GraphNode* node = getNodeAt(x, y);
                if (node != nullptr) {
                    g.RemoveNode(selectedNode);
                }
            }

            if (mode == MODE_NEWEDGE) {
                mode = MODE_NONE;
                qDebug() << "MODE_NONE";
            }
            update();
        }
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
        if (selectedNode != nullptr)
        {
            float w, h;
            QSize sz = this->size();

            w = sz.width();
            h = sz.height();
            int x = e->x();
            int y = e->y();

            if (x >= 0 && x <= w && y >= 0 && y <= w)
            {
                selectedNode->x = e->x()/w;
                selectedNode->y = e->y()/h;

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
        if (selectedEdge != nullptr ||
            selectedNode != nullptr)
        {
            if (mode == MODE_MOVE)
            {
                g.RemoveNode(selectedNode);
                selectedNode = nullptr;
                mode = MODE_NONE;
            }
            else if (mode == MODE_SELEDGE)
            {
                g.RemoveEdge(selectedEdge);
                emit edgeSelectionLoss();
                selectedEdge = nullptr;
                mode = MODE_NONE;
            }
            update();
        }
    }
}

void GraphEdit::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }

    int x = e->x();
    int y = e->y();

    GraphNode* node = getNodeAt(x, y);

    if (node != nullptr)
    {
        selectedNode = node;
        selectedEdge = nullptr;

        if (mode == MODE_SELEDGE) emit edgeSelectionLoss();
        mode = MODE_MOVE;
        emit nodeSelected(node);

        qDebug() << "MODE_MOVE";
    }
    else
    {
        this->AddNode(x, y);
        selectedNode = nullptr;
        selectedEdge = nullptr;
        mode = MODE_NONE;
        qDebug() << "MODE_NONE";
    }

    update();
}

GraphEdge* GraphEdit::GetSelectedEdge()
{
    if (mode == MODE_SELEDGE) return selectedEdge;
    else return nullptr;
}

void GraphEdit::SetEdgeWeight(GraphEdge* edge, int w)
{
    edge->w = w;
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

GraphNode* GraphEdit::getNodeAt(int x, int y)
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
            return &g.nodes[i];
        }
    }

    return nullptr;
}

GraphEdge* GraphEdit::getEdgeAt(int x, int y)
{
    float w, h;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();

    float rx = x/w;
    float ry = y/h;

    for (int i = 0; i < g.edges.length(); i++)
    {
        float x1 = g.edges[i].node1->x;
        float y1 = g.edges[i].node1->y;
        float x2 = g.edges[i].node2->x;
        float y2 = g.edges[i].node2->y;

        qDebug() << x1 << rx << x2 << y1 << ry << y2;

        if (rx <= std::max(x1, x2)+LINE_WIDTH && rx >= std::min(x1, x2)-LINE_WIDTH &&
            ry <= std::max(y1, y2)+LINE_WIDTH && ry >= std::min(y1, y2)-LINE_WIDTH)
        {
            float a = fabs((y2-y1)*rx-(x2-x1)*ry+x2*y1-y2*x1);
            float b = sqrt(pow(x2-x1, 2)+pow(y2-y1, 2));
            float d = a/b;

            qDebug() << "d = " << d;

            if (d <= LINE_WIDTH) return &g.edges[i];
        }
    }

    return nullptr;
}

bool GraphEdit::AddNode(int x, int y)
{
    GraphNode* prev = nullptr;
    GraphNode* end = nullptr;
    if (g.nodes.length()) {
        prev = &g.nodes.last();
    }

    int w, h;
    QSize sz = this->size();
    w = sz.width();
    h = sz.height();
    g.AddNode(float(x)/w, float(y)/h);

    end = &g.nodes.last();
    if (prev != nullptr && static_cast<MainWindow*>(QWidget::window())->autoConnectNodes()) {
        g.AddEdge(prev, end, 1);
    }

    return true;
}

Graph& GraphEdit::GetGraph()
{
    return g;
}

void GraphEdit::SetGraph(const Graph& src)
{
    g = src;
    update();
}

void GraphEdit::clearInternalState()
{
    mode = MODE_NONE;
    selectedEdge = nullptr;
    selectedNode = nullptr;
    // from, to = ;
}

/*QVector<GraphEdge> GraphEdit::GetEdges()
{
    return edges;
}

QVector<GraphNode> GraphEdit::GetNodes()
{
    return nodes;
}*/
