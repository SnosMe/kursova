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

signals:
    void backgroundChanged(bool isActive);

public slots:
    void changeBackground();
    void deleteBackground();

protected:
    void paintEvent(QPaintEvent*);

private:
    void updateCache();
};

#endif // GRAPHBACKGROUND_H
