#include "selectport.h"
#include "ui_selectport.h"

SelectPort::SelectPort(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::SelectPort),
    widget_mainWindow_(parent)
{
    ui->setupUi(this);
}

SelectPort::~SelectPort()
{
    delete ui;
}

void SelectPort::closeEvent(QCloseEvent* event)
{
    event->accept(); ///< We close the window
    if(widget_mainWindow_ != NULL)
        widget_mainWindow_->setEnabled(true);
}


void SelectPort::on_closeButton_clicked()
{
    close();
    if(widget_mainWindow_ != NULL)
        widget_mainWindow_->setEnabled(true);
}

