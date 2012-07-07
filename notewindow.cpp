#include "notewindow.h"
#include "ui_notewindow.h"

noteWindow::noteWindow(const QStringList& category, QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::noteWindow)
{
    ui->setupUi(this);

    defaultIndex = ui->fontComboBox->currentIndex();

    setupWindow(filename, category);

    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(setNewFont(QFont)));
}

void noteWindow::setupWindow(QString filename, QStringList category)
{
    QString line;

    ui->comboBox->insertItems(0, category);

    if(!filename.isEmpty())
    {
        QFile file(filename);

        file.open(QIODevice::ReadOnly | QIODevice::Text);

        if(file.isOpen())
        {
            QTextStream flux(&file);

            if(!flux.atEnd())
            {
                line = flux.readLine();

                ui->fontComboBox->setCurrentIndex(line.toInt());
                ui->textEdit->setFont(ui->fontComboBox->currentFont());

                line = flux.readLine();

                ui->lineEdit->setText(line);

                line = flux.readLine();

                ui->comboBox->setCurrentIndex(category.indexOf(line));

                while(!flux.atEnd())
                {
                    line = flux.readLine();
                    ui->textEdit->append(line);
                }
            }
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier : " + filename);
            return;
        }
        file.close();
    }
}

void noteWindow::setNewFont(QFont newf)
{
    ui->textEdit->setFont(newf);
}

noteWindow::~noteWindow()
{
    delete ui;
}

QString noteWindow::title() const
{
    return ui->lineEdit->text();
}

QString noteWindow::text() const
{
    return ui->textEdit->document()->toPlainText();
}

int noteWindow::currentFont() const
{
    return ui->fontComboBox->currentIndex();
}

QString noteWindow::category() const
{
    return ui->comboBox->currentText();
}
