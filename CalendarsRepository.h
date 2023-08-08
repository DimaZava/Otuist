#ifndef CALENDARSREPOSITORY_H
#define CALENDARSREPOSITORY_H

#include <set>

#include "CalendarItem.h"

class CalendarsRepository
{
public:
    CalendarsRepository();
    ~CalendarsRepository();

    const std::set<std::shared_ptr<CalendarItem>> getCalendars() const;

private:
    std::set<std::shared_ptr<CalendarItem>> connectedCalendars;
};

#endif // CALENDARSREPOSITORY_H
