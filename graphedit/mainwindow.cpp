#include <QStack>
#include <QDebug>
#include <QFileDialog>
#include <QtAlgorithms>
#include <QHash>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algorithm/dijkstra.h"
#include "io/graphwriter.h"
#include "io/graphreader.h"
#include "algorithm/fordfulkerson.h"
#include "algorithm/prim.h"
#include "algorithm/dijkstra.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    helpWindow = new Manual();
    devsWindow = new About_Dev();
}

MainWindow::~MainWindow()
{
    delete helpWindow;
    delete devsWindow;
    delete ui;
}

bool MainWindow::autoConnectNodes()
{
    return ui->autoConnectNodes->isChecked();
}

bool MainWindow::isDirectedGraph()
{
    return ui->directedGraph->isChecked();
}

void MainWindow::setDirectedGraph(bool directed)
{
    ui->directedGraph->setChecked(directed);
    ui->widget->GetGraph().directed = directed;
}

void MainWindow::on_lineEdit_textEdited(const QString &str)
{
    if (ui->widget->selectedEdge != nullptr) {
        int w  = str.toInt();
        if (w > 0) {
            ui->widget->selectedEdge->w = w;
            ui->widget->update();
        }
    } else if (ui->widget->selectedNode != nullptr) {
        ui->widget->selectedNode->name = str;
        ui->widget->update();
    }
}

void MainWindow::on_widget_edgeSelected(GraphEdge* edge)
{
    ui->lineEdit->setText(QString::number(edge->w));
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();
}

void MainWindow::on_widget_nodeSelected(GraphNode* node)
{
    ui->lineEdit->setText(node->name);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

    ui->btn_markBegin->setEnabled(true);
    ui->btn_markEnd->setEnabled(true);
    setBeginEndBtnsState();
}

void MainWindow::on_widget_edgeSelectionLoss()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->clear();
}

void MainWindow::on_widget_nodeSelectionLoss()
{
    on_widget_edgeSelectionLoss();

    ui->btn_markBegin->setEnabled(false);
    ui->btn_markEnd->setEnabled(false);
    setBeginEndBtnsState();
}

void MainWindow::on_btn_firstState_clicked()
{
    if (stateIdx != 1)
    {
        stateIdx = 1;
        ui->widget->clearInternalState();
        // this->on_widget_edgeSelectionLoss(); GLOBAL_STATE if ==
        ui->widget->SetGraph(states[stateIdx]);
    }
}

void MainWindow::on_btn_prevState_clicked()
{
    if (stateIdx > 1)
    {
        stateIdx--;
        ui->widget->clearInternalState();
        ui->widget->SetGraph(states[stateIdx]);
    }
}

void MainWindow::on_btn_nextState_clicked()
{
    if (stateIdx < states.length()-1)
    {
        stateIdx++;
        ui->widget->clearInternalState();
        ui->widget->SetGraph(states[stateIdx]);
    }
}

void MainWindow::on_btn_lastState_clicked()
{
    if (stateIdx != states.length()-1)
    {
        stateIdx = states.length()-1;
        ui->widget->clearInternalState();
        ui->widget->SetGraph(states[stateIdx]);
    }
}

void MainWindow::on_btn_dijkstra_clicked()
{
    beforeAlgorithm();
    
    Dijkstra algo(&states);
    algo.run();

    afterAlgorithm();
}

void MainWindow::on_btn_editGraph_clicked()
{
    ui->widget->SetGraph(states[0]);
    ui->widget->update();
    ui->toolsWidget->setCurrentWidget(ui->editTools);
}

void MainWindow::on_saveToFile_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Зберегти як GraphML", "", "GraphML (*.graphml)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QFile::WriteOnly);

        GraphWriter out(ui->widget->GetGraph(), file);
        out.writeToFile();

        file.close();
    }
}

void MainWindow::on_openFromFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Відкрити GraphML", "", "GraphML (*.graphml)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QFile::ReadOnly | QFile::Text);

        GraphReader in(file);
        ui->widget->SetGraph(in.readFromFile());
        setDirectedGraph(ui->widget->GetGraph().directed);

        file.close();
    }
}

void MainWindow::on_btn_markBegin_clicked()
{
    ui->widget->GetGraph().setBeginNode(ui->widget->selectedNode);
    setBeginEndBtnsState();
}

void MainWindow::on_btn_markEnd_clicked()
{
    ui->widget->GetGraph().setEndNode(ui->widget->selectedNode);
    setBeginEndBtnsState();
}

void MainWindow::setBeginEndBtnsState()
{
    if (ui->widget->selectedNode == nullptr)
    {
        ui->btn_markBegin->setChecked(false);
        ui->btn_markEnd->setChecked(false);
    }
    else
    {
        if (ui->widget->GetGraph().begin == ui->widget->selectedNode)
        {
            ui->btn_markBegin->setChecked(true);
            ui->btn_markEnd->setChecked(false);
        }
        else if (ui->widget->GetGraph().end == ui->widget->selectedNode)
        {
            ui->btn_markBegin->setChecked(false);
            ui->btn_markEnd->setChecked(true);
        }
        else
        {
            ui->btn_markBegin->setChecked(false);
            ui->btn_markEnd->setChecked(false);
        }
    }
}

void MainWindow::beforeAlgorithm()
{
    states.clear();
    ui->widget->clearInternalState();
    // add graph at [0] as original
    states.append(ui->widget->GetGraph());
}

void MainWindow::afterAlgorithm()
{
    stateIdx = 1;
    ui->widget->SetGraph(states[stateIdx]);
    ui->widget->update();
    ui->toolsWidget->setCurrentWidget(ui->viewTools);
}

void MainWindow::on_btn_clearAll_clicked()
{
    ui->widget->clearInternalState();
    ui->widget->SetGraph(Graph());
}

void MainWindow::on_directedGraph_stateChanged(int state)
{
    ui->widget->GetGraph().directed = (state == Qt::Checked);
}

void MainWindow::on_btn_bgImage_clicked()
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

        ui->graphBg->bgImage = newImg;
    }
}

void MainWindow::on_btn_primAlgo_clicked()
{
    beforeAlgorithm();

    Prim algo(&states);
    algo.run();

    afterAlgorithm();
}

void MainWindow::on_btn_kruskalAlgo_clicked()
{
    beforeAlgorithm();

    Graph gCopy = ui->widget->GetGraph();
    // algo
    int m = gCopy.edges.size();
    int n = gCopy.nodes.size();

    QVector<GraphEdge*> g;
    for (GraphEdge& edge : gCopy.edges)
        g.append(&edge);

    qSort(g.begin(), g.end(), GraphEdge::lessThan);

    QHash<GraphNode*, int> tree_id;
    for (int i=0; i<n; ++i)
        tree_id.insert(&gCopy.nodes[i], i);

    for (int i=0; i<m; ++i)
    {
        GraphNode* a = g[i]->node1;
        GraphNode* b = g[i]->node2;

        if (tree_id.value(a) != tree_id.value(b))
        {
            g[i]->node1->existInTrees = true;
            g[i]->node2->existInTrees = true;
            g[i]->node1->color = ColorMode::BOLD;
            g[i]->node2->color = ColorMode::BOLD;
            g[i]->color = ColorMode::BOLD;
            states.append(gCopy);

            int old_id = tree_id.value(b);
            int new_id = tree_id.value(a);
            QMutableHashIterator<GraphNode*, int> j(tree_id);
            while (j.hasNext())
            {
                j.next();
                if (j.value() == old_id)
                {
                    j.value() = new_id;
                }
            }
        }
    }

    afterAlgorithm();
}

void MainWindow::on_btn_fordaAlgo_clicked()
{
    beforeAlgorithm();

    states.append(states[0]); // TODO: delete this line after UI done
    FordFulkerson run(&states);

    afterAlgorithm();
}

void MainWindow::on_showHelp_triggered()
{
    helpWindow->show();
}

void MainWindow::on_showDevs_triggered()
{
    devsWindow->show();
}
