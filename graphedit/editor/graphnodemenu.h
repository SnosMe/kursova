#ifndef GRAPHNODEMENU_H
#define GRAPHNODEMENU_H

#include <QDialog>
#include "../graph.h"

namespace Ui {
class GraphNodeMenu;
}

class GraphNodeMenu : public QDialog
{
    Q_OBJECT

public:
    explicit GraphNodeMenu(QWidget *parent = 0);
    ~GraphNodeMenu();

    void initNode(const QString& name, bool isBegin, bool isEnd);
    QString getName();
    bool isBegin();
    bool isEnd();

private slots:
    void on_btn_delete_clicked();

private:
    Ui::GraphNodeMenu *ui;
};

#endif // GRAPHNODEMENU_H
