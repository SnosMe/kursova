#ifndef GRAPHEDGEMENU_H
#define GRAPHEDGEMENU_H

#include <QDialog>

namespace Ui {
class GraphEdgeMenu;
}

class GraphEdgeMenu : public QDialog
{
    Q_OBJECT

public:
    explicit GraphEdgeMenu(QWidget *parent = 0);
    ~GraphEdgeMenu();

    void initEdge(int weight);
    int getWeight();

private slots:
    void on_btn_delete_clicked();

private:
    Ui::GraphEdgeMenu *ui;
};

#endif // GRAPHEDGEMENU_H
