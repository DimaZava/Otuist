#include "CalendarsRepository.h"

CalendarsRepository::CalendarsRepository()
    : connectedCalendars(std::set<std::shared_ptr<CalendarItem>>{})
{
    connectedCalendars.emplace(new CalendarItem{"Default", {"Work", "Home", "Family"}});
}

CalendarsRepository::~CalendarsRepository()
{
    while (!connectedCalendars.empty())
    {
        auto ptr = (*connectedCalendars.begin());
        ptr.reset();
        connectedCalendars.erase(connectedCalendars.begin());
    }
}

const std::set<std::shared_ptr<CalendarItem>> CalendarsRepository::getCalendars() const
{
    return connectedCalendars;
}
