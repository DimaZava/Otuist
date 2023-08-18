#ifndef EVENTSWINDOW_H
#define EVENTSWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include "../../BusinessLogicLayer/CalendarsManager/CalendarsManager.h" // remove if not needed
#include "../Calendar/CalendarWidget.h"
#include "../Calendar/CalendarsScrollWidget.h"
#include "../Events/EventsScrollWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EventsWindow; }
QT_END_NAMESPACE

class EventsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EventsWindow(QWidget* parent = nullptr);
    ~EventsWindow() override;

    void readPositionSizeSettings();

private:
    Ui::EventsWindow *ui;
    std::shared_ptr<CalendarsManager> calendarsManager;

    std::unique_ptr<QSplitter> mainContentLayout;
    std::unique_ptr<QSplitter> eventsContentLayout;
    std::unique_ptr<CalendarsScrollWidget> calendarsScrollWidget;
    std::unique_ptr<CalendarWidget> calendarWidget;
    std::unique_ptr<EventsScrollWidget> eventsScrollWidget;

    void configureLayout();
    void setupInitialState();
    void setupSignals();
    void writePositionSizeSettings();

    void moveEvent(QMoveEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};
#endif // EVENTSWINDOW_H
