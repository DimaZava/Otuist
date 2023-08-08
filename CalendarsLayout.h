#ifndef CALENDARSLAYOUT_H
#define CALENDARSLAYOUT_H

#include "CalendarItem.h"

#include <QLabel>
#include <QTreeWidget>
#include <QVBoxLayout>

class CalendarsLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    CalendarsLayout(QWidget* parent = nullptr);
    ~CalendarsLayout();

    void setCalendarItems(const std::set<std::shared_ptr<CalendarItem>>& calendarItems);

private:
    std::set<std::shared_ptr<CalendarItem>> calendarItems;
    std::unique_ptr<QTreeWidget> categoriesTree;

    void configureLayout();
    void reloadData();
};

#endif // CALENDARSLAYOUT_H
