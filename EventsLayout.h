#ifndef EVENTSLAYOUT_H
#define EVENTSLAYOUT_H

#include "CalendarEvent.h"
#include "Observer.h"

#include <QTextEdit> // todo: remove when not needed
#include <QVBoxLayout>
#include <set>

class EventsLayout
    : public QVBoxLayout
    , public Observer
{
    Q_OBJECT
public:
    explicit EventsLayout(Subject& subject, QWidget* parent = nullptr);
    ~EventsLayout() override;

    void setCalendarEvents(const std::set<std::shared_ptr<CalendarEvent>>& calendarEvents);

    void update(Subject&) override;

private:
    QTextEdit* tmp;
    std::set<std::shared_ptr<CalendarEvent>> calendarEvents;

    void configureLayout();
    void reloadData();
};

#endif // EVENTSLAYOUT_H
