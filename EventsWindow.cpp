#include "EventsWindow.h"
#include "./ui_EventsWindow.h"

EventsWindow::EventsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EventsWindow)
{
    ui->setupUi(this);
}

EventsWindow::~EventsWindow()
{
    delete ui;
}

