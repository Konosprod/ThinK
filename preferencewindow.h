#ifndef PREFERENCEWINDOW_H
#define PREFERENCEWINDOW_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class preferenceWindow;
}

class preferenceWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit preferenceWindow(QWidget *parent = 0);
    ~preferenceWindow();
    QString getPath() const;

public slots:
    void chooseIcon();
    
private:
    Ui::preferenceWindow *ui;
    QString path;
};

#endif // PREFERENCEWINDOW_H
