#include "graphbackground.h"
#include <QPainter>

GraphBackground::GraphBackground(QWidget *parent)
    : QWidget(parent)
{
}

void GraphBackground::paintEvent(QPaintEvent *)
{
    if (!bgImage.isNull())
    {
        QPainter p(this);

        if (oldSize != this->size())
        {
            oldSize = this->size();
            cache = QPixmap::fromImage(bgImage.scaled(this->size(), Qt::KeepAspectRatioByExpanding));
            p.drawPixmap(QPoint(0, 0), cache);
        }
        else
        {
            p.drawPixmap(QPoint(0, 0), cache);
        }
    }
}
