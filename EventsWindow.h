#ifndef EVENTSWINDOW_H
#define EVENTSWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class EventsWindow; }
QT_END_NAMESPACE

class EventsWindow : public QMainWindow
{
    Q_OBJECT

public:
    EventsWindow(QWidget *parent = nullptr);
    ~EventsWindow();

private:
    Ui::EventsWindow *ui;
};
#endif // EVENTSWINDOW_H
