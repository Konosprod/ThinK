#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notewindow.h"
#include "categorywindow.h"
#include "preferencewindow.h"

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    sticon = new QSystemTrayIcon(QIcon(":ressources/icon"), this);

    checkPreferences();

    QString dir(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/Non Classees")));
    QDir directory(dir);

    if(!directory.exists(dir))
    {
        directory.mkpath(dir);
    }

    QFile file(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/category")));

    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
    }

    file.close();

    categoryModel = 0;
    noteModel = 0;

    listCategory();
    scanNote();
    setupConnexions();

}

void mainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        sticon->hide();
        show();
    break;

    default:
        ;
    }
}

void mainWindow::mini()
{
    sticon->show();
    hide();
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::scanNote()
{
    notes = new QStringList[category.count()];

    for(int i = 0; i < category.count(); i++)
    {
        QDir notedir(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + category.at(i) + "/")));

        if(!notedir.exists())
        {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le dossier : " +
                                  QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + category.at(i) + "/")));
        }
        else
        {
            notes[i] += notedir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        }
    }
}

void mainWindow::checkPreferences()
{
    QString path(QDir::toNativeSeparators(QDir::homePath() + "/.ThinK/pref"));
    QString str;
    QFile f(path);

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream s(&f);
    str = s.readLine();

    if(str != "")
    {
        sticon->setIcon(QIcon(str));
        setWindowIcon(QIcon(str));
    }
}

void mainWindow::preferences()
{
    preferenceWindow* pf = new preferenceWindow(this);

    QString path(QDir::toNativeSeparators(QDir::homePath()+"/.ThinK/pref"));

    if(pf->exec() == QDialog::Accepted)
    {
        QFile f(path);
        f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream stream(&f);

        if(!pf->getPath().isEmpty())
        {
            stream << pf->getPath();
        }

        f.close();
    }

    checkPreferences();
}

void mainWindow::setupConnexions()
{
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(preferences()));
    connect(ui->listCategory, SIGNAL(clicked(QModelIndex)), this, SLOT(showNotes(QModelIndex)));
    connect(ui->listNote, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openNote(QModelIndex)));
    connect(ui->actionNewNote, SIGNAL(triggered()), this, SLOT(newNote()));
    connect(ui->actionDelNote, SIGNAL(triggered()), this, SLOT(delNote()));
    connect(ui->actionNewBlocNote, SIGNAL(triggered()), this, SLOT(newBlocNote()));
    connect(ui->actionDelBlocNote, SIGNAL(triggered()), this, SLOT(delBlocNote()));
    connect(ui->actionMini, SIGNAL(triggered()), this, SLOT(mini()));
    connect(sticon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void mainWindow::openNote(QModelIndex index)
{
    QString mess(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + category[ui->listCategory->currentIndex().row()] + "/" + notes[ui->listCategory->currentIndex().row()].at(index.row()))));
    noteWindow *nw = new noteWindow(category, mess, this);

    if(nw->exec() == QDialog::Accepted)
    {
        QString title;
        QString text;
        QFile file(mess);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(nw->title().isEmpty())
        {
            title = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
        else
        {
            title = nw->title();
        }
        text = nw->text();
        QTextStream stream(&file);
        stream << nw->currentFont() << "\n" << title << "\n" << nw->category() << "\n" << text;
        QString newName(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + nw->category() + "/" + title)));
        file.rename(mess, newName);
        file.close();
    }

    scanNote();

}

void mainWindow::showNotes(QModelIndex index)
{
    if(noteModel != 0)
    {
        delete noteModel;
        noteModel = 0;
    }
    noteModel = new QStringListModel(notes[index.row()]);
    ui->listNote->setModel(noteModel);
    ui->listNote->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void mainWindow::listCategory()
{
    category.clear();
    category << "Non Classees";

    QFile file(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/category")));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&file);

    while (!flux.atEnd())
    {
           category << flux.readLine();
    }

    if(categoryModel != 0)
    {
        delete categoryModel;
        categoryModel = 0;
    }

    categoryModel = new QStringListModel(category, this);
    ui->listCategory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listCategory->setModel(categoryModel);

    file.close();
}

bool mainWindow::removeDir(const QString &dirPath)
{
    QDir folder(dirPath);
    folder.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    foreach(QFileInfo fileInfo, folder.entryInfoList())
    {
        if(fileInfo.isDir())
        {
            if(!removeDir(fileInfo.filePath()))
                return false;
        }
        else if(fileInfo.isFile())
        {
            if(!QFile::remove(fileInfo.filePath()))
            {
                QMessageBox::information(this, "Erreur", "Impossible de supprimer le fichier : " + fileInfo.filePath());
                return false;
            }
        }
    }

    if(!folder.rmdir(dirPath))
    {
        QMessageBox::information(this, "Erreur", "Impossible de supprimer le dossier : " + dirPath);
        return false;
    }

    return true;
}

void mainWindow::delBlocNote()
{
    if(ui->listCategory->currentIndex().isValid())
    {
        removeDir(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + category[ui->listCategory->currentIndex().row()] + "/")));
        category.removeOne(category[ui->listCategory->currentIndex().row()]);
        category.removeDuplicates();
        category.removeOne("Non Classees");
        QFile cat(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/category")));
        cat.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream stream(&cat);

        foreach(QString s, category)
        {
            stream << s << "\n";
        }
    }
    else
    {
        QMessageBox::information(this, "Erreur", "Vous devez d'abord selectionner la categorie a suppimer !");
    }

    listCategory();
    scanNote();
}

void mainWindow::delNote()
{
    if(ui->listNote->currentIndex().isValid() && ui->listCategory->currentIndex().isValid())
    {
        QModelIndex index = ui->listNote->currentIndex();
        QFile file(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + category[ui->listCategory->currentIndex().row()] + "/" + notes[ui->listCategory->currentIndex().row()].at(index.row()))));
        file.remove();
    }
    else
    {
        QMessageBox::information(this, "Erreur", "Vous devez d'abord selectionner la note a supprimer !");
    }

    scanNote();
}

void mainWindow::newBlocNote()
{
    categoryWindow* cw = new categoryWindow(this);

    if(cw->exec() == QDialog::Accepted)
    {
        QFile cat(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/category")));
        cat.open(QIODevice::Append | QIODevice::Text);

        QTextStream stream(&cat);
        if(!cw->text().isEmpty())
        {
            stream << cw->text() << "\n";
            QString dir(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + cw->text() + "/")));
            QDir directory(dir);

            if(!directory.exists(dir))
            {
                directory.mkdir(dir);
            }
        }
        cat.close();
    }

    listCategory();
    scanNote();

}

void mainWindow::newNote()
{
    noteWindow* nw = new noteWindow(category, "", this);

    if(nw->exec() == QDialog::Accepted)
    {
        QString title;
        QString text;
        if(nw->title().isEmpty())
        {
            title = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
        else
        {
            title = nw->title();
        }
        text = nw->text();
        QFile file(QDir::toNativeSeparators((QDir::homePath() + "/.ThinK/" + nw->category() + "/" + title)));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&file);
        stream << nw->currentFont() << "\n" << title << "\n" << nw->category() << "\n" << text;
        file.close();
    }

    scanNote();
}
