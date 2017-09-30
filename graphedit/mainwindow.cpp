#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    //ui->lineEdit->setText("123456");
    ui->widget->AddNode(0.25, 0.25, 1);
    ui->widget->AddNode(0.25, 0.75, 2);
    ui->widget->AddNode(0.75, 0.25, 50);
    ui->widget->AddNode(0.75, 0.75, 100);

    ui->widget->AddEdge(0,1);
    ui->widget->AddEdge(2,1);
    ui->widget->AddEdge(3,1);
    ui->widget->AddEdge(2,3);

    ui->widget->update();
}
