#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class noteWindow;
}

class noteWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit noteWindow(const QStringList& category,QString filename = "", QWidget *parent = 0);
    QString title() const;
    QString text() const;
    int currentFont() const;
    QString category() const;
    ~noteWindow();

public slots:
    void setNewFont(QFont);

protected:
    void setupWindow(QString filename, QStringList category);
    
private:
    Ui::noteWindow *ui;
    int defaultIndex;
};

#endif // NOTEWINDOW_H
