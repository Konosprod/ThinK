#ifndef CATEGORYWINDOW_H
#define CATEGORYWINDOW_H

#include <QDialog>

namespace Ui {
class categoryWindow;
}

class categoryWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit categoryWindow(QWidget *parent = 0);
    ~categoryWindow();
    QString text() const;
    
private:
    Ui::categoryWindow *ui;
};

#endif // CATEGORYWINDOW_H
