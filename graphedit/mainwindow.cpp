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

void MainWindow::on_widget_twoNodesSelected(int, int)
{
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_widget_dblSelectionLoss()
{
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    int w = QVariant(ui->lineEdit->text()).toUInt();
    int a = ui->widget->GetSelected(0);
    int b = ui->widget->GetSelected(1);
    ui->widget->AddEdge(a, b, w);
}
