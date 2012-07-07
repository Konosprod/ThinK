#include "preferencewindow.h"
#include "ui_preferencewindow.h"

preferenceWindow::preferenceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferenceWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(chooseIcon()));
}

preferenceWindow::~preferenceWindow()
{
    delete ui;
}

void preferenceWindow::chooseIcon()
{
    path = QFileDialog::getOpenFileName(this, "Choisissez votre icon", QDir::homePath());
}

QString preferenceWindow::getPath() const
{
    return path;
}
