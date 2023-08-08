#ifndef EVENTSWINDOW_H
#define EVENTSWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include "CalendarsRepository.h" // remove if not needed
#include "CalendarsScrollWidget.h"
#include "EventsCalendarWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EventsWindow; }
QT_END_NAMESPACE

class EventsWindow : public QMainWindow
{
    Q_OBJECT

public:
    EventsWindow(std::shared_ptr<CalendarsRepository> calendarsRepository, QWidget* parent = nullptr);
    ~EventsWindow();

    void readPositionSizeSettings();

private:
    Ui::EventsWindow *ui;
    std::unique_ptr<QSplitter> mainContentLayout;
    std::unique_ptr<CalendarsScrollWidget> foldersScrollWidget;
    std::unique_ptr<EventsCalendarWidget> calendar;

    std::shared_ptr<CalendarsRepository> calendarsRepository;

    void configureLayout();
    void setupInitialState();
    void setupSignals();
    void writePositionSizeSettings();

    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent* event);
};
#endif // EVENTSWINDOW_H
