#ifndef EVENTSLAYOUT_H
#define EVENTSLAYOUT_H

#include "../../BusinessLogicLayer/CalendarsManager/CalendarsManager.h"
#include "../../BusinessLogicLayer/Observer.h"
#include "../../ModelsLayer/CalendarEvent/CalendarEvent.h"
#include "../../PresentationLayer/Calendar/CalendarWidget.h"
#include "EventWidget.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <set>

class EventsLayout
    : public QVBoxLayout
    , public EventWidgetDelegate
    , public IObserver<std::set<SharedCalendarEvent>>
    , public IObserver<CalendarSelectionDTO>
{
    Q_OBJECT
public:
    explicit EventsLayout(
        const std::shared_ptr<CalendarsManager>& calendarsManager,
        ISubject<CalendarSelectionDTO>& calendarSubject,
        QWidget* parent = nullptr);
    ~EventsLayout() override;

    void setCalendarEvents(const std::set<SharedCalendarEvent>& calendarEvents);
    void didChange(const std::set<SharedCalendarEvent>& value) override;
    void didChange(const CalendarSelectionDTO& value) override;

    // EventWidgetDelegate
    void removeEventButtonDidClick(const SharedCalendarEvent& event) override;
    void eventUpdateDidFinish() override;

private:
    const std::unique_ptr<QListWidget> eventsList;
    const std::shared_ptr<CalendarsManager> calendarsManager;
    std::set<SharedCalendarEvent, CalendarEventComparator> calendarEvents;

    void configureLayout();
    void reloadData();
    void cleanItems();
};

#endif // EVENTSLAYOUT_H
