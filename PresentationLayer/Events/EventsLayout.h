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
    , public IObserver<std::set<std::shared_ptr<CalendarEvent>>>
    , public IObserver<CalendarSelectionDTO>
{
    Q_OBJECT
public:
    explicit EventsLayout(
        const std::shared_ptr<CalendarsManager>& calendarsManager,
        ISubject<CalendarSelectionDTO>& calendarSubject,
        QWidget* parent = nullptr);
    ~EventsLayout() override;

    void setCalendarEvents(const std::set<std::shared_ptr<CalendarEvent>>& calendarEvents);
    void didChange(const std::set<std::shared_ptr<CalendarEvent>>& value) override;
    void didChange(const CalendarSelectionDTO& value) override;

    // EventWidgetDelegate
    void removeEventButtonDidClick(const std::shared_ptr<CalendarEvent>& event) override;

private:
    const std::unique_ptr<QListWidget> eventsList;
    const std::shared_ptr<CalendarsManager> calendarsManager;
    std::set<std::shared_ptr<CalendarEvent>, CalendarEventComparator> calendarEvents;

    void configureLayout();
    void reloadData();
    void cleanItems();
};

#endif // EVENTSLAYOUT_H
