#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include "Event.h"

#include <set>
#include <string>

class CalendarItem
{
public:
    CalendarItem(std::string name, std::set<std::string> categories, std::vector<Event> events = {});
    ~CalendarItem();

    std::string getName() const;
    std::set<std::string> getCategories() const;
    std::vector<Event> getEvents() const;

private:
    std::string name;
    std::set<std::string> categories;
    std::vector<Event> events;
};

#endif // CALENDARITEM_H
