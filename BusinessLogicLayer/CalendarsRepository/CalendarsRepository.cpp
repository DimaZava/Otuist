#include "CalendarsRepository.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"

#include <ranges>

CalendarsRepository::CalendarsRepository(){
    const auto now = std::chrono::system_clock::now();

    std::set<std::shared_ptr<CalendarEvent>> events;
    events.insert(std::make_shared<CalendarEvent>(
        "Event Work 1", "Work", now, CommonUtils::Time::endOfDate(now), "Some Work Event 1"));
    events.insert(std::make_shared<CalendarEvent>(
        "Event Work 2",
        "Work",
        now + std::chrono::hours(10),
        CommonUtils::Time::endOfDate(now + std::chrono::hours(10)),
        "Some Work Event 2"));

    events.insert(std::make_shared<CalendarEvent>(
        "Event Home 1",
        "Home",
        now + std::chrono::days(1),
        CommonUtils::Time::endOfDate(now + std::chrono::days(1)),
        "Some Home Event 1"));
    events.insert(std::make_shared<CalendarEvent>(
        "Event Home 2",
        "Home",
        now + std::chrono::days(1) + std::chrono::hours(10),
        CommonUtils::Time::endOfDate(now + std::chrono::days(1) + std::chrono::hours(10)),
        "Some Home Event 2"));

    events.insert(std::make_shared<CalendarEvent>(
        "Event Family 1",
        "Family",
        now + std::chrono::days(2),
        CommonUtils::Time::endOfDate(now + std::chrono::days(2)),
        "Some Family Event 1"));
    events.insert(std::make_shared<CalendarEvent>(
        "Event Family 2",
        "Family",
        now + std::chrono::days(2) + std::chrono::hours(10),
        CommonUtils::Time::endOfDate(now + std::chrono::days(2) + std::chrono::hours(10)),
        "Some Family Event 2"));

    addObject(std::make_shared<CalendarItem>("Default", std::set<std::string>{"Work", "Home", "Family"}, events));
}

CalendarsRepository::~CalendarsRepository()
{
    qDebug() << __PRETTY_FUNCTION__;
    auto connectedCalendars = readObjects();
    while (!connectedCalendars.empty())
    {
        auto ptr = (*connectedCalendars.begin());
        ptr.reset();
        connectedCalendars.erase(connectedCalendars.begin());
    }
}

// Calendars CRUD

void CalendarsRepository::addCalendar(const std::shared_ptr<CalendarItem>& calendar)
{
    addObject(calendar);
    notify(getEvents());
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
    // notify();
}

void CalendarsRepository::deleteCalendar(const std::string& name)
{
    deleteObject(name);
    notify(getEvents());
}

// Events CRUD

std::set<std::shared_ptr<CalendarEvent>> CalendarsRepository::getEvents(
    const std::optional<std::chrono::time_point<std::chrono::system_clock>> beginDateTime,
    const std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime,
    const std::optional<std::set<std::string>> calendarNames) const
{
    return getEventsBetweenDatesForCalendars(beginDateTime, endDateTime, calendarNames);
}

void CalendarsRepository::reloadEvents()
{
    notify(getEvents(CommonUtils::Time::beginOfDate(), CommonUtils::Time::endOfDate()));
}

// Private methods

std::set<std::shared_ptr<CalendarEvent>> CalendarsRepository::getEventsBetweenDatesForCalendars(
    const std::optional<std::chrono::time_point<std::chrono::system_clock>> beginDateTime,
    const std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime,
    const std::optional<std::set<std::string>> calendarNames) const
{
    std::set<std::shared_ptr<CalendarItem>> connectedCalendars = readObjects();
    if (calendarNames.has_value())
    {
        auto unwrappedCalendarNames = calendarNames.value();
        auto result = connectedCalendars |
            std::ranges::views::filter([&unwrappedCalendarNames](const std::shared_ptr<CalendarItem>& calendarItem) {
                          return unwrappedCalendarNames.contains(calendarItem->getName());
                      });
        connectedCalendars = std::set<std::shared_ptr<CalendarItem>>{result.begin(), result.end()};
    }

    std::set<std::shared_ptr<CalendarEvent>> collectedEvents;
    for (const auto& calendar : connectedCalendars)
    {
        auto events = calendar->getEvents();
        auto result = events |
            std::ranges::views::filter(
                          [&events, &beginDateTime, &endDateTime](const std::shared_ptr<CalendarEvent>& event) {
            bool passedBeginTime = true;
            if (beginDateTime.has_value())
            {
                passedBeginTime = event->getBeginDateTime() >= beginDateTime.value();
            }

            bool passedEndTime = true;
            if (endDateTime.has_value())
            {
                passedEndTime =
                    event->getEndDateTime().has_value() && event->getEndDateTime().value() <= endDateTime.value() ||
                    !event->getEndDateTime().has_value();
            }
            return passedBeginTime && passedEndTime;
            });
        collectedEvents.insert(result.begin(), result.end());
    }
    return collectedEvents;
}
