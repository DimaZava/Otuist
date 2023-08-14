#include "CalendarsRepository.h"

CalendarsRepository::CalendarsRepository(){
    const auto now = std::chrono::system_clock::now();

    CalendarEvent workEvent1{"Event Work 2", "Work", now, std::nullopt, "Some Work Event 1"};
    CalendarEvent workEvent2{"Event Work 2", "Work", now + std::chrono::days(1), std::nullopt, "Some Work Event 2"};

    CalendarEvent homeEvent1{"Event Home 1", "Home", now, std::nullopt, "Some Home Event 1"};
    CalendarEvent homeEvent2{"Event Home 2", "Home", now + std::chrono::hours(25), std::nullopt, "Some Home Event 2"};

    CalendarEvent familyEvent1{"Family Home 1", "Family", now, std::nullopt, "Some Family Event 1"};
    CalendarEvent familyEvent2{
        "Event Family 2", "Family", now + std::chrono::hours(26), std::nullopt, "Some Family Event 2"};

    std::set<CalendarEvent> events;
    events.insert(workEvent1);
    events.insert(workEvent2);
    events.insert(homeEvent1);
    events.insert(homeEvent2);
    events.insert(familyEvent1);
    events.insert(familyEvent2);

    // small workaround to fix make_shared init list issue
    addCalendar(std::make_shared<CalendarItem>("Default", std::set<std::string>{"Work", "Home", "Family"}, events));
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
    notify();
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
    notify();
}

void CalendarsRepository::deleteCalendar(const std::string& name)
{
    deleteObject(name);
    notify();
}
