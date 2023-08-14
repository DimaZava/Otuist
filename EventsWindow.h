#ifndef EVENTSWINDOW_H
#define EVENTSWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include "CalendarWidget.h"
#include "CalendarsRepository.h" // remove if not needed
#include "CalendarsScrollWidget.h"
#include "EventsScrollWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EventsWindow; }
QT_END_NAMESPACE

class EventsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EventsWindow(const std::shared_ptr<CalendarsRepository>& calendarsRepository, QWidget* parent = nullptr);
    ~EventsWindow() override;

    void readPositionSizeSettings();

private:
    Ui::EventsWindow *ui;
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
