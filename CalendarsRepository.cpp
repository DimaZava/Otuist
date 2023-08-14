#include "CalendarsRepository.h"

CalendarsRepository::CalendarsRepository(){
    // small workaround to fix make_shared init list issue
    addCalendar(std::make_shared<CalendarItem>("Default", std::set<std::string>{"Work", "Home", "Family"}));
}

CalendarsRepository::~CalendarsRepository()
{
    auto connectedCalendars = readObjects();
    while (!connectedCalendars.empty())
    {
        auto ptr = (*connectedCalendars.begin());
        ptr.reset();
        connectedCalendars.erase(connectedCalendars.begin());
    }
}

void CalendarsRepository::addCalendar(const std::shared_ptr<CalendarItem>& calendar)
{
    addObject(calendar);
}

std::set<std::shared_ptr<CalendarItem>> CalendarsRepository::getCalendars() const
{
    return readObjects();
}

std::optional<std::shared_ptr<CalendarItem>> CalendarsRepository::getCalendar(const std::string& name) const
{
    return readObject(name);
}

void CalendarsRepository::updateCalendar(const std::string& name, const std::shared_ptr<CalendarItem>& calendar)
{
    // updateObject(name, calendar);
}

void CalendarsRepository::deleteCalendar(const std::string& name)
{
    deleteObject(name);
}
