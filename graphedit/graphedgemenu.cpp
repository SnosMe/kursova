#include "graphedgemenu.h"
#include "ui_graphedgemenu.h"

GraphEdgeMenu::GraphEdgeMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphEdgeMenu)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setFixedSize(QSize(300, 320));

    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->btn_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

GraphEdgeMenu::~GraphEdgeMenu()
{
    delete ui;
}

void GraphEdgeMenu::initEdge(int weight)
{
    ui->edgeWeight->setValue(weight);
    ui->edgeWeight->setFocus();
}

int GraphEdgeMenu::getWeight()
{
    return ui->edgeWeight->value();
}

void GraphEdgeMenu::on_btn_delete_clicked()
{
    this->done(2);
}
