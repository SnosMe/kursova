#include <algorithm>
#include <cmath>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>
#include "../mainwindow.h"
#include "graphedit.h"
#include "graphnodemenu.h"
#include "graphedgemenu.h"

//const float LINE_WIDTH = 0.05;

static int dist(int x, int y)
{
    return (int) qSqrt(x*x+y*y);
}

static qreal dist(QPointF p)
{
    return qSqrt(p.x()*p.x()+p.y()*p.y());
}

GraphEdit::GraphEdit(QWidget *parent) : QWidget(parent)
{
    mode = MODE_NONE;
    selectedEdge = nullptr;
    selectedNode = nullptr;
    viewport.setRect(-1.0, -1.0, 2, 2);
    setFocusPolicy(Qt::StrongFocus);
}

void GraphEdit::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    int font_size = (int) (20.0/std::min(viewport.width(), viewport.height()));

    p.setRenderHints(QPainter::Antialiasing);
    p.setFont(QFont("Consolas", font_size));

    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    //r = std::min(w, h)*0.1;
    r = std::min(w/viewport.width(), h/viewport.height())*0.2;

    for (int i = 0; i < g.edges.length(); i++)
    {
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

        QPointF p1 = toScreenCoords(QPointF(g.edges[i].node1->x, g.edges[i].node1->y));
        QPointF p2 = toScreenCoords(QPointF(g.edges[i].node2->x, g.edges[i].node2->y));
        p.drawLine(p1, p2);

        if (g.directed)
        {
            QPointF v = p1-p2;
            v = v*r/dist(v)/2;
            QPointF v1;
            QPointF v2;
            QPointF end = p2+v;

            const qreal ANGLE = 0.393;

            v1.setX(v.x()*qCos(ANGLE)-v.y()*qSin(ANGLE));
            v1.setY(v.y()*qCos(ANGLE)+v.x()*qSin(ANGLE));
            v2.setX(v.x()*qCos(ANGLE)+v.y()*qSin(ANGLE));
            v2.setY(v.y()*qCos(ANGLE)-v.x()*qSin(ANGLE));

            p.drawLine(end, end+v1);
            p.drawLine(end, end+v2);
        }

        QString text;
        if (g.edges[i].flow) {
            text = QString("%1(%2)").arg(QString::number(g.edges[i].w), QString::number(g.edges[i].flow));
        } else {
            text = QString::number(g.edges[i].w);
        }
        p.setPen(QPen(QColor(33, 33, 33), 1.0));
        p.drawText((p1+p2)/2.0, text);
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
            if (g.begin == &g.nodes[i] ||
                g.end == &g.nodes[i])
            {
                p.setPen(QPen(QColor(33, 33, 33), 2.0, Qt::DashDotDotLine));
            }
            else
            {
                p.setPen(QPen(QColor(33, 33, 33), 1.0));
            }
            p.setBrush(QBrush(QColor(255, 255, 255)));
        }

        QPointF px = toScreenCoords(QPointF(g.nodes[i].x, g.nodes[i].y));
        p.drawEllipse(px, r/2, r/2);

        p.setPen(textPan);
        p.drawText(QRectF(px.x()-r/2, px.y()-r/2, r, r), Qt::AlignCenter, g.nodes[i].name);
    }
}

void GraphEdit::mousePressEvent(QMouseEvent* e)
{
    int x = e->x();
    int y = e->y();

    if (e->button() == Qt::LeftButton)
    {
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
    else if (e->button() == Qt::MidButton)
    {
        from = QPoint(x, y);
        mode = MODE_MOVE_VP;
        emit edgeSelectionLoss();
        emit nodeSelectionLoss();
        update();
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
                openNodeDialog(selectedNode);
            }
            else if (mode == MODE_SELEDGE)
            {
                openEdgeDialog(selectedEdge);
            }
            else
            {
                int x = e->x();
                int y = e->y();

                GraphNode* node = getNodeAt(x, y);
                if (node != nullptr) {
                    openNodeDialog(node);
                }
            }

            if (mode == MODE_NEWEDGE) {
                mode = MODE_NONE;
                qDebug() << "MODE_NONE";
            }
            update();
        }
    }
    else if (e->button() == Qt::MidButton)
    {
        QSizeF sz = viewport.size();
        viewport.setLeft(viewport.left()+(from.x()-e->x())*sz.width()/size().width());
        viewport.setTop(viewport.top()+(from.y()-e->y())*sz.height()/size().height());
        viewport.setSize(sz);
        mode = MODE_NONE;
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
        if (selectedNode != nullptr)
        {
            QSize sz = this->size();

            float w = sz.width();
            float h = sz.height();
            int x = e->x();
            int y = e->y();

            if (x >= 0 && x <= w && y >= 0 && y <= h)
            {
                QPointF p = toInternalCoords(QPointF(x, y));

                selectedNode->x = p.x();
                selectedNode->y = p.y();

                update();
            }
        }
    }
    else if (mode == MODE_MOVE_VP)
    {
        QSizeF sz = viewport.size();
        viewport.setLeft(viewport.left()+(from.x()-e->x())*sz.width()/size().width());
        viewport.setTop(viewport.top()+(from.y()-e->y())*sz.height()/size().height());
        viewport.setSize(sz);
        from = QPoint(e->x(), e->y());
        update();
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

void GraphEdit::wheelEvent(QWheelEvent* e)
{
    e->accept();
    int v = e->angleDelta().y()/120;

    if (v > 0)
    {
        viewport.setLeft(viewport.left() + viewport.width()/10.0);
        viewport.setTop(viewport.top() + viewport.height()/10.0);
        viewport.setWidth(viewport.width()*0.8);
        viewport.setHeight(viewport.height()*0.8);
    }
    else if (v < 0)
    {
        viewport.setWidth(viewport.width()*1.25);
        viewport.setHeight(viewport.height()*1.25);
        viewport.setLeft(viewport.left() - viewport.width()/10.0);
        viewport.setTop(viewport.top() - viewport.height()/10.0);
    }

    qDebug() << viewport;
    update();
}

GraphNode* GraphEdit::getNodeAt(int x, int y)
{
    int w, h, r;
    QSize sz = this->size();

    w = sz.width();
    h = sz.height();
    r = std::min(w/viewport.width(), h/viewport.height())*0.2/2;

    for (int i = 0; i < g.nodes.length(); i++)
    {
        QPointF p = toScreenCoords(QPointF(g.nodes[i].x, g.nodes[i].y));
        int d = dist(p.x() - x, p.y() - y);
        if (d <= r)
        {
            return &g.nodes[i];
        }
    }

    return nullptr;
}

GraphEdge* GraphEdit::getEdgeAt(int x, int y)
{
    QPointF p = toInternalCoords(QPointF(x,y));

    float rx = p.x();
    float ry = p.y();

    float LINE_WIDTH = std::min(viewport.width(), viewport.height())*0.025;

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

    QPointF p = toInternalCoords(QPointF(x, y));

    g.AddNode(p.x(), p.y());

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
    emit edgeSelectionLoss();
    emit nodeSelectionLoss();
}

void GraphEdit::setSelectedObjectText(const QString& str)
{
    if (selectedEdge != nullptr)
    {
        int w = str.toInt();
        if (w > 0) {
            selectedEdge->w = w;
            update();
        }
    }
    else if (selectedNode != nullptr) {
        selectedNode->name = str;
        update();
    }
}

void GraphEdit::setIsDirectedGraph(int state)
{
    g.directed = (state == Qt::Checked);
    update();
}

void GraphEdit::openNodeDialog(GraphNode* node)
{
    GraphNodeMenu dialog(this);
    dialog.initNode(node->name, node == g.begin, node == g.end);
    int result = dialog.exec();

    if (result == 1)
    {
        node->name = dialog.getName();
        if (dialog.isBegin()) {
            g.setBeginNode(node);
        } else if (dialog.isEnd()) {
            g.setEndNode(node);
        }
        update();
    }
    else if (result == 2)
    {
        g.RemoveNode(node);
        if (node == selectedNode)
        {
            selectedNode = nullptr;
            emit nodeSelectionLoss();
            mode = MODE_NONE;
        }
        update();
    }
}

void GraphEdit::openEdgeDialog(GraphEdge* edge)
{
    GraphEdgeMenu dialog(this);
    dialog.initEdge(edge->w);
    int result = dialog.exec();

    if (result == 1)
    {
        edge->w = dialog.getWeight();
        update();
    }
    else if (result == 2)
    {
        g.RemoveEdge(selectedEdge);
        if (edge == selectedEdge)
        {
            selectedEdge = nullptr;
            emit edgeSelectionLoss();
            mode = MODE_NONE;
        }
        update();
    }
}

QPointF GraphEdit::toScreenCoords(QPointF pos)
{
    QSize sz = this->size();
    qreal x = (pos.x()-viewport.left())*sz.width()/viewport.width();
    qreal y = (pos.y()-viewport.top())*sz.height()/viewport.height();
    return QPointF(x, y);
}

QPointF GraphEdit::toInternalCoords(QPointF pos)
{
    QSize sz = this->size();
    qreal x = pos.x()*viewport.width()/sz.width()+viewport.left();
    qreal y = pos.y()*viewport.height()/sz.height()+viewport.top();
    return QPointF(x, y);
}
