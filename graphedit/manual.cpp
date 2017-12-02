#include "manual.h"
#include "ui_manual.h"

Manual::Manual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manual)
{
    ui->setupUi(this);
}

Manual::~Manual()
{
    delete ui;
}

void Manual::on_Content_clicked()
{
    QStringList names;
    QString title = ui->Content->currentIndex().data(Qt::DisplayRole).toString();
    names << "Загальні відомості" << "Інтерфейс" << "Малювання графу"
          << "Прима, Краскала" << "Дейкстри" << "Форда-Фалкерсона"
          << "Кроки алгоритмів" << "Зображення підкладка" << "Збереження і відкриття";
    switch (names.indexOf(title))
    {
        case 0:
                ui->help_widget->setCurrentWidget(ui->aboutCommon);
            break;
        case 1:
                ui->help_widget->setCurrentWidget(ui->aboutInterface);
            break;
        case 2:
                ui->help_widget->setCurrentWidget(ui->aboutDrawing);
            break;
        case 3:
                ui->help_widget->setCurrentWidget(ui->aboutPrim_kraskala);
            break;
        case 4:
                ui->help_widget->setCurrentWidget(ui->aboutDijkstra);
            break;
        case 5:
                ui->help_widget->setCurrentWidget(ui->aboutFordfalkerson);
            break;
        case 6:
                ui->help_widget->setCurrentWidget(ui->aboutSteps);
            break;
        case 7:
                ui->help_widget->setCurrentWidget(ui->aboutPicture);
            break;
        case 8:
                ui->help_widget->setCurrentWidget(ui->aboutSaving);
            break;
    }

}

void Manual::on_pushButton_2_clicked()
{
    ui->help_widget->setCurrentIndex(ui->help_widget->currentIndex()+1);
}

void Manual::on_pushButton_clicked()
{
    ui->help_widget->setCurrentIndex(ui->help_widget->currentIndex()-1);
}
