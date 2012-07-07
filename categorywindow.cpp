#include "categorywindow.h"
#include "ui_categorywindow.h"

categoryWindow::categoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::categoryWindow)
{
    ui->setupUi(this);
}

categoryWindow::~categoryWindow()
{
    delete ui;
}

QString categoryWindow::text() const
{
    return ui->lineEdit->text();
}
