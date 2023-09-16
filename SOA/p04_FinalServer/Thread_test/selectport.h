#ifndef SELECTPORT_H
#define SELECTPORT_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class SelectPort;
}

class SelectPort : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectPort(QMainWindow *parent = nullptr);
    ~SelectPort();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_closeButton_clicked();

private:
    Ui::SelectPort *ui;
    QMainWindow* widget_mainWindow_; ///< It's neccesary to manipulate the main window from this object
};

#endif // SELECTPORT_H
