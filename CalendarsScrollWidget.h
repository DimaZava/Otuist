#ifndef CALENDARSSCROLLWIDGET_H
#define CALENDARSSCROLLWIDGET_H

#include "CalendarsLayout.h"

#include <QScrollArea>
#include <QVBoxLayout>

class CalendarsScrollWidget : public QScrollArea
{
    Q_OBJECT
public:
    CalendarsScrollWidget(QWidget* parent = nullptr);
    ~CalendarsScrollWidget();

    void setCalendarItems(const std::set<std::shared_ptr<CalendarItem>>& calendarItems);

private:
    std::unique_ptr<QWidget> calendarsLayoutContainer;
    std::unique_ptr<CalendarsLayout> calendarsLayout;

    void configureLayout();
};

#endif // CALENDARSSCROLLWIDGET_H
