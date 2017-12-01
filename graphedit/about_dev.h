#ifndef ABOUT_DEV_H
#define ABOUT_DEV_H

#include <QWidget>

namespace Ui {
class About_Dev;
}

class About_Dev : public QWidget
{
    Q_OBJECT

public:
    explicit About_Dev(QWidget *parent = 0);
    ~About_Dev();

private:
    Ui::About_Dev *ui;
};

#endif // ABOUT_DEV_H
