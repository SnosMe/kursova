#include <QStack>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dijkstra.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    /* GraphNode* prev = nullptr;
    GraphNode* end = nullptr;
    if (ui->widget->GetGraph().nodes.length()) {
        prev = &ui->widget->GetGraph().nodes.last();
    }

    ui->widget->AddNode();
    end = &ui->widget->GetGraph().nodes.last();

    if (prev != nullptr && ui->autoConnectNodes->isChecked()) {
        ui->widget->GetGraph().AddEdge(prev, end, 1);
    } */
}

void MainWindow::on_pushButton_2_clicked()
{
    GraphEdge* edge = ui->widget->GetSelectedEdge();
    int w  = ui->lineEdit->text().toInt();
    ui->widget->SetEdgeWeight(edge, w);
    ui->widget->update();
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
    ui->pushButton_2->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();
}

void MainWindow::on_widget_nodeSelected(GraphNode* node)
{
    ui->lineEdit->setText(node->name);
    ui->pushButton_2->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();
}

void MainWindow::on_widget_edgeSelectionLoss()
{
    ui->pushButton_2->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->clear();
}

void MainWindow::on_widget_nodeSelectionLoss()
{
    on_widget_edgeSelectionLoss();
}

void MainWindow::on_pushButton_3_clicked()
{
   /* Graph gr = ui->widget->GetGraph();
   QStack<int> st;

   st.push(1);

   states.clear();

    while(!st.isEmpty())
    {
        int cur = st.pop();
        QVector<GraphEdge> inc = gr.GetIncidentEdges(cur);
        for (int i = 0; i < inc.length(); i++)
        {
            if (inc[i].node1 != cur && !gr.GetNodeByID(inc[i].node1).colored)
            {
                st.push(inc[i].node1);
            }
            else if (inc[i].node2 != cur && !gr.GetNodeByID(inc[i].node2).colored)
            {
                st.push(inc[i].node2);
            }
        }

        gr.GetNodeByID(cur).colored = true;

        states.push_back(gr);
    }

    cur_st = 0;
    ui->widget->SetGraph(states[0]);

    ui->widget->update(); */
}

void MainWindow::on_btn_firstState_clicked()
{
    if (stateIdx != 0)
    {
        stateIdx = 0;
        ui->widget->clearInternalState();
        // this->on_widget_edgeSelectionLoss(); GLOBAL_STATE if ==
        ui->widget->SetGraph(states[stateIdx]);
    }
}

void MainWindow::on_btn_prevState_clicked()
{
    if (stateIdx > 0)
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
        states.clear();

        Graph gCopy = ui->widget->GetGraph();
        QVector<DijkstraGraph> dGraphArr;
        DijkstraGraph dGraph = DijkstraGraph();
        GraphNode* startVert = &gCopy.nodes[0];

        GraphEdge* edge = gCopy.getLowestEdge(gCopy.GetIncidentEdges(startVert))[0];
        GraphNode* end = (edge->node1 == startVert) ? edge->node2 : edge->node1;
        end->length = edge->w;
        edge->node1->existInTrees = true;
        edge->node2->existInTrees = true;
        dGraph.nodes.append(edge->node1);
        dGraph.nodes.append(edge->node2);
        dGraph.end = end;
        dGraphArr.append(dGraph);

        startVert->color = ColorMode::BOLD;
        states.append(gCopy);

        end->color = ColorMode::BOLD;
        edge->color = ColorMode::BOLD;
        states.append(gCopy);
        qDebug() << ": (" << edge->node1->id << ", " << edge->node2->id << ")\n";

        while (true) {
            GraphEdge* lowestEdge = nullptr;
            int lowestLength = INT_MAX;
            DijkstraGraph* foundInGraph = nullptr;
            for (auto& dGraph : dGraphArr) {
                for (auto node : dGraph.nodes) {
                    QVector<GraphEdge*> edges = gCopy.getLowestEdge(gCopy.GetIncidentEdges(node));
                    for (auto edge : edges) {
                        if (edge->node1->existInTrees && edge->node2->existInTrees)
                            continue;
                        int pathLen = (edge->node1->length) ? (edge->node1->length + edge->w) : (edge->node2->length + edge->w);
                        if ((edge->node1 == startVert) || (edge->node2 == startVert)) pathLen = edge->w;
                        if ((lowestEdge == nullptr) || (pathLen < lowestLength)) {
                            lowestEdge = edge;
                            lowestLength = pathLen;
                            foundInGraph = &dGraph;
                        }
                    }
                }
            }

            GraphNode* end = (lowestEdge->node1->existInTrees) ? lowestEdge->node2 : lowestEdge->node1;
            GraphNode* prev = (lowestEdge->node1->existInTrees) ? lowestEdge->node1 : lowestEdge->node2;
            if ((foundInGraph->end != prev) || (lowestEdge->node1 == startVert) || (lowestEdge->node2 == startVert)) {
                if ((lowestEdge->node1 == startVert) || (lowestEdge->node2 == startVert)) {
                    dGraphArr.append(DijkstraGraph());
                    foundInGraph = &dGraphArr.last();
                    foundInGraph->nodes.append(startVert);
                } else {
                    dGraphArr.append(DijkstraGraph(*foundInGraph));
                    foundInGraph = &dGraphArr.last();
                    for (int i = foundInGraph->nodes.size(); i != 1; i--) {
                        if (foundInGraph->nodes[i-1] != prev) {
                            foundInGraph->nodes.removeLast();
                        } else {
                            break;
                        };
                    }
                }
            }
            foundInGraph->nodes.append(end);
            foundInGraph->end = end;
            end->length = lowestLength;
            end->existInTrees = true;

            lowestEdge->color = ColorMode::BOLD;
            end->color = ColorMode::BOLD;
            states.append(gCopy);

            qDebug() << ": (" << lowestEdge->node1->id << ", " << lowestEdge->node2->id << ")\n";
            if (end == &gCopy.nodes.last()) {
                GraphNode* prev = nullptr;
                for (auto node : foundInGraph->nodes) {
                    gCopy.GetNodeByID(node->id)->color = ColorMode::GREEN;
                    if (prev != nullptr) {
                        for (auto& edge : gCopy.edges) {
                            if ((edge.node1 == prev && edge.node2 == node) ||
                                (edge.node1 == node && edge.node2 == prev)) {
                                edge.color = ColorMode::GREEN;
                            }
                        }
                    }
                    prev = node;
                }
                states.append(gCopy);
                break;
            }
        }

        stateIdx = 0;
        ui->widget->SetGraph(states[stateIdx]);
        ui->widget->update();
}
