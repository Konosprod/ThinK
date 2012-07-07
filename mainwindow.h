#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

protected:
    void scanNote();
    void listCategory();
    void setupConnexions();
    void checkPreferences();
    bool removeDir(const QString& path);

public slots:
    void preferences();
    void showNotes(QModelIndex);
    void openNote(QModelIndex);
    void newBlocNote();
    void newNote();
    void delNote();
    void delBlocNote();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void mini();

private:
    Ui::mainWindow *ui;
    QStringList category;
    QStringList* notes;
    QStringListModel* noteModel;
    QStringListModel* categoryModel;
    QSystemTrayIcon *sticon;
};

#endif // MAINWINDOW_H
