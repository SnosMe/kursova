#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "editor/graphedit.h"
#include "manual.h"
#include "about_dev.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool autoConnectNodes();
    bool isDirectedGraph();
    void setDirectedGraph(bool);

private slots:
    void on_btn_firstState_clicked();
    void on_btn_prevState_clicked();
    void on_btn_nextState_clicked();
    void on_btn_lastState_clicked();

    void on_btn_editGraph_clicked();
    void on_btn_clearAll_clicked();
    void on_graphBg_backgroundChanged(bool isActive);

    void on_saveToFile_triggered();
    void on_openFromFile_triggered();

    void on_btn_primAlgo_clicked();
    void on_btn_kruskalAlgo_clicked();
    void on_btn_dijkstra_clicked();
    void on_btn_fordaAlgo_clicked();

    void on_showHelp_triggered();
    void on_showDevs_triggered();

private:
    Ui::MainWindow* ui;
    Manual* helpWindow;
    About_Dev* devsWindow;
    QList<Graph> states;
    int stateIdx;
    void beforeAlgorithm();
    void afterAlgorithm();
    void updateState();
};

#endif // MAINWINDOW_H
