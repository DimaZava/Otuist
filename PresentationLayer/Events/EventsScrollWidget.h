#ifndef EVENTSSCROLLWIDGET_H
#define EVENTSSCROLLWIDGET_H

#include "../../BusinessLogicLayer/Observer.h"
#include "../../PresentationLayer/Calendar/CalendarWidget.h"
#include "EventsLayout.h"

#include <QListWidget>
#include <QScrollArea>
#include <set>

class EventsScrollWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit EventsScrollWidget(
        const std::shared_ptr<CalendarsManager>& calendarsManager,
        ISubject<CalendarSelectionDTO>& calendarSubject,
        QWidget* parent = nullptr);
    ~EventsScrollWidget() override;

private:
    std::unique_ptr<QWidget> eventsLayoutContainer;
    std::unique_ptr<EventsLayout> eventsLayout;

    void configureLayout();
};

#endif // EVENTSSCROLLWIDGET_H
