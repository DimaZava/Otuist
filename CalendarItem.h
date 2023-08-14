#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <set>
#include <string>

#include "CalendarEvent.h"
#include "NamedObjectInterface.h"

class CalendarItem : NamedObjectInterface
{
public:
    CalendarItem(
        const std::string& name,
        const std::set<std::string>& categories,
        const std::set<CalendarEvent>& events = {});
    CalendarItem(const CalendarItem& calendar);
    CalendarItem(CalendarItem&& calendar) noexcept;
    ~CalendarItem() override;

    std::string getName() const override;
    std::set<std::string> getCategories() const;
    std::set<CalendarEvent> getEvents() const;

    std::string getUuid() const;

    CalendarItem& operator=(const CalendarItem& other);
    CalendarItem& operator=(CalendarItem&& other) noexcept;

private:
    std::string uuid;
    std::string name;
    std::set<std::string> categories;
    std::set<CalendarEvent> events;
};

#endif // CALENDARITEM_H
