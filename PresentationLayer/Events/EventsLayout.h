#ifndef EVENTSLAYOUT_H
#define EVENTSLAYOUT_H

#include "../../BusinessLogicLayer/CalendarsRepository/CalendarsRepository.h"
#include "../../BusinessLogicLayer/Observer.h"
#include "../../ModelsLayer/CalendarEvent/CalendarEvent.h"
#include "../../PresentationLayer/Calendar/CalendarWidget.h"

#include <QTextEdit> // todo: remove when not needed
#include <QVBoxLayout>
#include <set>

class EventsLayout
    : public QVBoxLayout
    , public IObserver<std::set<std::shared_ptr<CalendarEvent>>>
    , public IObserver<CalendarSelectionDTO>
{
    Q_OBJECT
public:
    explicit EventsLayout(
        const std::shared_ptr<CalendarsRepository>& calendarsRepository,
        ISubject<CalendarSelectionDTO>& calendarSubject,
        QWidget* parent = nullptr);
    ~EventsLayout() override;

    void setCalendarEvents(const std::set<std::shared_ptr<CalendarEvent>>& calendarEvents);
    void didChange(const std::set<std::shared_ptr<CalendarEvent>>& value) override;
    void didChange(const CalendarSelectionDTO& value) override;

private:
    QTextEdit* tmp;
    const std::shared_ptr<CalendarsRepository> calendarsRepository;
    std::set<std::shared_ptr<CalendarEvent>> calendarEvents;

    void configureLayout();
    void reloadData();
};

#endif // EVENTSLAYOUT_H
