#include "about_dev.h"
#include "ui_about_dev.h"

About_Dev::About_Dev(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About_Dev)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
}

About_Dev::~About_Dev()
{
    delete ui;
}
