#include "graphnodemenu.h"
#include "ui_graphnodemenu.h"

GraphNodeMenu::GraphNodeMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphNodeMenu)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setFixedSize(QSize(300, 320));

    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->btn_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

GraphNodeMenu::~GraphNodeMenu()
{
    delete ui;
}

void GraphNodeMenu::initNode(const QString& name, bool isBegin, bool isEnd)
{
    ui->nodeName->setText(name);
    ui->nodeName->setFocus();
    ui->nodeName->selectAll();

    if (isBegin) {
        ui->btn_markBegin->setChecked(true);
    }
    else if (isEnd) {
        ui->btn_markEnd->setChecked(true);
    }
}

QString GraphNodeMenu::getName()
{
    return ui->nodeName->text();
}

bool GraphNodeMenu::isBegin()
{
    return ui->btn_markBegin->isChecked();
}

bool GraphNodeMenu::isEnd()
{
    return ui->btn_markEnd->isChecked();
}

void GraphNodeMenu::on_btn_delete_clicked()
{
    this->done(2);
}
