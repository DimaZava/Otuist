#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <set>
#include <string>

#include "Event.h"
#include "NamedObjectInterface.h"

class CalendarItem : NamedObjectInterface
{
public:
    CalendarItem(std::string name, std::set<std::string> categories, std::vector<Event> events = {});
    CalendarItem(const CalendarItem& calendar);
    CalendarItem(CalendarItem&& calendar) noexcept;
    ~CalendarItem() override;

    std::string getName() const override;
    std::set<std::string> getCategories() const;
    std::vector<Event> getEvents() const;

    std::string getUuid() const;

    CalendarItem& operator=(const CalendarItem& other);
    CalendarItem& operator=(CalendarItem&& other) noexcept;

private:
    std::string uuid;
    std::string name;
    std::set<std::string> categories;
    std::vector<Event> events;
};

#endif // CALENDARITEM_H
