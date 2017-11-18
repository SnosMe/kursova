#include <QStack>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //ui->lineEdit->setText("123456");
    ui->widget->AddNode();
  /*  ui->widget->AddNode(0.25, 0.75, 2);
    ui->widget->AddNode(0.75, 0.25, 50);
    ui->widget->AddNode(0.75, 0.75, 100);

    ui->widget->AddEdge(0,1,5);
    ui->widget->AddEdge(2,1,6);
    ui->widget->AddEdge(3,1,8);
    ui->widget->AddEdge(2,3,10);*/

   // ui->widget->update();
}

/*void MainWindow::on_widget_twoNodesSelected(int, int)
{
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_widget_dblSelectionLoss()
{

}*/

void MainWindow::on_pushButton_2_clicked()
{
    GraphEdge* edge = ui->widget->GetSelectedEdge();
    int w  = ui->lineEdit->text().toInt();
    ui->widget->SetEdgeWeight(edge, w);
    ui->lineEdit->setText(QString(""));
    ui->widget->update();
}

void MainWindow::on_widget_edgeSelected(GraphEdge* edge)
{
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_widget_edgeSelectionLoss()
{
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::on_pushButton_3_clicked()
{
   Graph gr = ui->widget->GetGraph();
   QStack<int> st;

   /*st.push(1);

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
    ui->widget->SetGraph(states[0]);*/

    ui->widget->update();
}

void MainWindow::on_pushButton_4_clicked()
{
    if (cur_st > 0)
    {
        cur_st--;
        ui->widget->SetGraph(states[cur_st]);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if (cur_st < states.length()-1)
    {
        cur_st++;
        ui->widget->SetGraph(states[cur_st]);
    }
}
