#include "CalendarsRepository.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"

#include <algorithm>
#include <ranges>

CalendarsRepository::CalendarsRepository(){
    const auto now = std::chrono::system_clock::now();

    std::set<std::shared_ptr<CalendarEvent>> events;
    events.insert(std::make_shared<CalendarEvent>(
        "Event Work 1",
        "Default",
        "Work",
        now,
        std::nullopt,
        /*CommonUtils::Time::endOfDate(now),*/ "Some Work Event 1"));
    events.insert(std::make_shared<CalendarEvent>(
        "Event Work 2",
        "Default",
        "Work",
        now + std::chrono::hours(10),
        std::nullopt, // CommonUtils::Time::endOfDate(now + std::chrono::hours(10)),
        "Some Work Event 2"));

    events.insert(std::make_shared<CalendarEvent>(
        "Event Home 1",
        "Default",
        "Home",
        now + std::chrono::days(1),
        std::nullopt, // CommonUtils::Time::endOfDate(now + std::chrono::days(1)),
        "Some Home Event 1"));
    events.insert(std::make_shared<CalendarEvent>(
        "Event Home 2",
        "Default",
        "Home",
        now + std::chrono::days(1) + std::chrono::hours(10),
        std::nullopt, // CommonUtils::Time::endOfDate(now + std::chrono::days(1) + std::chrono::hours(10)),
        "Some Home Event 2"));

    events.insert(std::make_shared<CalendarEvent>(
        "Event Family 1",
        "Default",
        "Family",
        now + std::chrono::days(2),
        std::nullopt, // CommonUtils::Time::endOfDate(now + std::chrono::days(2)),
        "Some Family Event 1"));
    events.insert(std::make_shared<CalendarEvent>(
        "Event Family 2",
        "Default",
        "Family",
        now + std::chrono::days(2) + std::chrono::hours(10),
        std::nullopt, // CommonUtils::Time::endOfDate(now + std::chrono::days(2) + std::chrono::hours(10)),
        "Some Family Event 2"));

    std::set<std::shared_ptr<CalendarCategory>, CalendarCategoryComparator> categories;
    categories.insert(std::make_shared<CalendarCategory>("Work"));
    categories.insert(std::make_shared<CalendarCategory>("Home"));
    categories.insert(std::make_shared<CalendarCategory>("Family"));

    addObject(std::make_shared<CalendarItem>("Default", categories, events));
}

CalendarsRepository::~CalendarsRepository()
{
    qDebug() << __PRETTY_FUNCTION__;
}

// Calendars CRUD

void CalendarsRepository::addCalendar(const std::shared_ptr<CalendarItem>& calendar)
{
    addObject(calendar);
    reloadEvents();
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
    reloadEvents();
}

void CalendarsRepository::setCalendarsCategoryActive(
    const std::string& calendarName,
    const std::string& categoryName,
    bool isActive) const
{
    auto calendar = getCalendar(calendarName);
    if (!calendar.has_value())
        return;

    auto categories = calendar.value()->getCategories();
    auto findClosure = [&categoryName](const std::shared_ptr<CalendarCategory>& category) {
        return category->name == categoryName;
    };
    auto it = std::find_if(categories.begin(), categories.end(), findClosure);

    if (it != categories.end())
    {
        it->get()->isActive = isActive;
    }
    reloadEvents();
}

// Events CRUD

void CalendarsRepository::addEvent(const std::shared_ptr<CalendarEvent>& event) const
{
    auto calendar = getCalendar(event->getCalendarName())->get();
    calendar->addEvent(event);
    reloadEvents();
}
void CalendarsRepository::removeEvent(const std::shared_ptr<CalendarEvent>& event) const
{
    auto calendar = getCalendar(event->getCalendarName())->get();
    calendar->removeEvent(event);
    reloadEvents();
}

std::set<std::shared_ptr<CalendarEvent>> CalendarsRepository::getEvents(
    const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
    const std::chrono::time_point<std::chrono::system_clock> endDateTime)
{
    activeDatesFrame.updateDates(beginDateTime, endDateTime);
    return getEventsBetweenDatesForCalendars(beginDateTime, endDateTime);
}

void CalendarsRepository::reloadEvents() const
{
    notify(getEventsBetweenDatesForCalendars(activeDatesFrame.beginDateTime, activeDatesFrame.endDateTime));
}

// Private methods

std::set<std::shared_ptr<CalendarEvent>> CalendarsRepository::getEventsBetweenDatesForCalendars(
    const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
    const std::chrono::time_point<std::chrono::system_clock> endDateTime) const
{
    auto activeCalendars = getCalendars();

    std::set<std::shared_ptr<CalendarEvent>> collectedEvents;
    for (const auto& calendar : activeCalendars)
    {
        auto activeActualEvents = calendar->getEvents();
        auto categories = calendar->getCategories();

        auto activeCategoryNamesReducer =
            [&categories](std::set<std::string>& partialResult, const std::shared_ptr<CalendarCategory>& category) {
            if (category->isActive)
                partialResult.insert(category->name);
            return partialResult;
        };
        std::set<std::string> activeCategoryNames =
            std::reduce(categories.begin(), categories.end(), std::set<std::string>{}, activeCategoryNamesReducer);

        auto activeActualEventsFilter = [&activeActualEvents, &beginDateTime, &endDateTime, &activeCategoryNames](
                                            const std::shared_ptr<CalendarEvent>& event) {
            bool passedBeginTime = event->getBeginDateTime() >= beginDateTime;

            bool isPassedSingleDayEvent = !event->getEndDateTime().has_value() &&
                CommonUtils::Time::isBetweenDates(event->getBeginDateTime(), beginDateTime, endDateTime);

            bool passedEndTime = !event->getEndDateTime().has_value() && isPassedSingleDayEvent ||
                event->getEndDateTime().has_value() && event->getEndDateTime().value() <= endDateTime;

            return passedBeginTime && passedEndTime && activeCategoryNames.contains(event->getCategory());
        };

        auto result = activeActualEvents | std::ranges::views::filter(activeActualEventsFilter);
        collectedEvents.insert(result.begin(), result.end());
    }
    return collectedEvents;
}
