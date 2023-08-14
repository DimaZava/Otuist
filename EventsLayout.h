#ifndef EVENTSLAYOUT_H
#define EVENTSLAYOUT_H

#include "CalendarEvent.h"
#include "Observer.h"

#include <QTextEdit> // todo: remove when not needed
#include <QVBoxLayout>
#include <set>

class EventsLayout
    : public QVBoxLayout
    , public Observer<std::set<std::shared_ptr<CalendarEvent>>>
{
    Q_OBJECT
public:
    explicit EventsLayout(Subject<std::set<std::shared_ptr<CalendarEvent>>>& subject, QWidget* parent = nullptr);
    ~EventsLayout() override;

    void setCalendarEvents(const std::set<std::shared_ptr<CalendarEvent>>& calendarEvents);
    void notify(const std::set<std::shared_ptr<CalendarEvent>>& value) override;

private:
    QTextEdit* tmp;
    std::set<std::shared_ptr<CalendarEvent>> calendarEvents;

    void configureLayout();
    void reloadData();
};

#endif // EVENTSLAYOUT_H
