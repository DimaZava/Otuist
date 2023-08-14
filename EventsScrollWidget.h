#ifndef EVENTSSCROLLWIDGET_H
#define EVENTSSCROLLWIDGET_H

#include "EventsLayout.h"
#include "Observer.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <set>

class EventsScrollWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit EventsScrollWidget(Subject<std::set<std::shared_ptr<CalendarEvent>>>& subject, QWidget* parent = nullptr);
    ~EventsScrollWidget() override;

private:
    std::unique_ptr<QWidget> eventsLayoutContainer;
    std::unique_ptr<EventsLayout> eventsLayout;

    void configureLayout();
};

#endif // EVENTSSCROLLWIDGET_H
