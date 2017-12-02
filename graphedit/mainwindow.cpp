#include <QStack>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "io/graphwriter.h"
#include "io/graphreader.h"
#include "algorithm/fordfulkerson.h"
#include "algorithm/prim.h"
#include "algorithm/dijkstra.h"
#include "algorithm/kruskal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btn_deleteBgImage->setVisible(false);
    connect(this->ui->btn_bgImage, SIGNAL(clicked()), this->ui->graphBg, SLOT(changeBackground()));
    connect(this->ui->btn_deleteBgImage, SIGNAL(clicked()), this->ui->graphBg, SLOT(deleteBackground()));
    connect(this->ui->lineEdit, SIGNAL(textEdited(QString)), this->ui->widget, SLOT(setSelectedObjectText(QString)));
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
    updateBeginEndBtnsState();
}

void MainWindow::on_widget_edgeSelectionLoss()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->clear();
}

void MainWindow::on_widget_nodeSelectionLoss()
{
    // lineEdit is common for edge and node
    on_widget_edgeSelectionLoss();

    ui->btn_markBegin->setEnabled(false);
    ui->btn_markEnd->setEnabled(false);
    updateBeginEndBtnsState();
}

void MainWindow::on_btn_firstState_clicked()
{
    if (stateIdx != 1)
    {
        stateIdx = 1;
        updateState();
    }
}

void MainWindow::on_btn_prevState_clicked()
{
    if (stateIdx > 1)
    {
        stateIdx--;
        updateState();
    }
}

void MainWindow::on_btn_nextState_clicked()
{
    if (stateIdx < states.length()-1)
    {
        stateIdx++;
        updateState();
    }
}

void MainWindow::on_btn_lastState_clicked()
{
    if (stateIdx != states.length()-1)
    {
        stateIdx = states.length()-1;
        updateState();
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
    updateBeginEndBtnsState();
}

void MainWindow::on_btn_markEnd_clicked()
{
    ui->widget->GetGraph().setEndNode(ui->widget->selectedNode);
    updateBeginEndBtnsState();
}

void MainWindow::updateBeginEndBtnsState()
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
    stateIdx = 0;
    ui->label_countSteps->setText(QString::number(states.length()-1));
    on_btn_firstState_clicked();
    ui->toolsWidget->setCurrentWidget(ui->viewTools);
}

void MainWindow::updateState()
{
    ui->widget->clearInternalState();
    ui->widget->SetGraph(states[stateIdx]);
    ui->label_currentStep->setText(QString::number(stateIdx));
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

    Kruskal algo(&states);
    algo.run();

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

void MainWindow::on_graphBg_backgroundChanged(bool isActive)
{
    if (isActive)
    {
        ui->btn_bgImage->setVisible(false);
        ui->btn_deleteBgImage->setVisible(true);
    }
    else
    {
        ui->btn_deleteBgImage->setVisible(false);
        ui->btn_bgImage->setVisible(true);
    }
}
