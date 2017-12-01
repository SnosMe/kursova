#ifndef MANUAL_H
#define MANUAL_H

#include <QWidget>
#include <QTreeWidgetItem>
namespace Ui {
class Manual;
}

class Manual : public QWidget
{
    Q_OBJECT

public:
    explicit Manual(QWidget *parent = 0);
    ~Manual();

private slots:
    void on_Content_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::Manual *ui;
};

#endif // MANUAL_H
