#include "graphbackground.h"
#include <QPainter>
#include <QFileDialog>
#include <QDebug>

GraphBackground::GraphBackground(QWidget *parent)
    : QWidget(parent)
{
}

void GraphBackground::changeBackground()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Відкрити зображення", "", "Файли зображень (*.png *.jpg *.bmp)");

    if (!fileName.isEmpty())
    {
        QImage original(fileName);
        original = original.convertToFormat(QImage::Format_ARGB32);

        QImage newImg(original.size(), QImage::Format_ARGB32);
        newImg.fill(Qt::transparent);

        QPainter painter(&newImg);
        painter.setOpacity(0.1);
        painter.drawImage(QRect(0, 0, original.width(), original.height()), original);

        this->bgImage = newImg;
        emit backgroundChanged(true);

        updateCache();
        update();
    }
}

void GraphBackground::deleteBackground()
{
    this->bgImage = QImage();
    emit backgroundChanged(false);
    update();
}

void GraphBackground::paintEvent(QPaintEvent *)
{
    if (!bgImage.isNull())
    {
        QPainter p(this);

        if (oldSize != this->size())
        {
            updateCache();
            p.drawPixmap(QPoint(0, 0), cache);
        }
        else
        {
            p.drawPixmap(QPoint(0, 0), cache);
        }
    }
}

void GraphBackground::updateCache()
{
    oldSize = this->size();
    cache = QPixmap::fromImage(bgImage.scaled(this->size(), Qt::KeepAspectRatioByExpanding));
}
