#ifndef GRAPHBACKGROUND_H
#define GRAPHBACKGROUND_H

#include <QWidget>

class GraphBackground : public QWidget
{
    Q_OBJECT
public:
    explicit GraphBackground(QWidget *parent = nullptr);
    QImage bgImage;
    QSize oldSize;
    QPixmap cache;

protected:
    void paintEvent(QPaintEvent*);
};

#endif // GRAPHBACKGROUND_H
