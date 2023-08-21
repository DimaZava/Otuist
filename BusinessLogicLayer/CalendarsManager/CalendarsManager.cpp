#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "CalendarsManager.h"

#include <algorithm>
#include <ranges>

CalendarsManager::CalendarsManager()
    : notificationsManager(std::make_unique<NotificationManager>())
{
    const auto now = std::chrono::system_clock::now();

    std::set<SharedCalendarEvent> events;
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

CalendarsManager::~CalendarsManager()
{
    qDebug() << __PRETTY_FUNCTION__;
}

// Calendars CRUD

void CalendarsManager::addCalendar(const SharedCalendarItem& calendar)
{
    addObject(calendar);
    reloadEvents();
}

std::set<SharedCalendarItem> CalendarsManager::getCalendars() const
{
    return readObjects();
}

std::optional<SharedCalendarItem> CalendarsManager::getCalendar(const std::string& name) const
{
    return readObject(name);
}

void CalendarsManager::updateCalendar(const std::string& name, const SharedCalendarItem& calendar)
{
    // updateObject(name, calendar);
    // notify();
}

void CalendarsManager::deleteCalendar(const std::string& name)
{
    deleteObject(name);
    reloadEvents();
}

void CalendarsManager::setCalendarsCategoryActive(
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

void CalendarsManager::addEvent(const SharedCalendarEvent& event) const
{
    auto calendar = getCalendar(event->getCalendarName())->get();
    calendar->addEvent(event);
    reloadEvents();

    notificationsManager->scheduleNotification(event);
}

void CalendarsManager::removeEvent(const SharedCalendarEvent& event) const
{
    notificationsManager->unscheduleNotification(event);

    auto calendar = getCalendar(event->getCalendarName())->get();
    calendar->removeEvent(event);
    reloadEvents();
}

void CalendarsManager::removeEvents(const std::set<SharedCalendarEvent>& events) const
{
    for (const auto& event : events)
    {
        auto calendar = getCalendar(event->getCalendarName())->get();
        calendar->removeEvent(event);
    }
    reloadEvents();
}

std::set<SharedCalendarEvent> CalendarsManager::getEvents(
    const DateTime& beginDateTime,
    const std::optional<DateTime>& endDateTime,
    bool shouldUpdateActiveDatesFrame)
{
    const DateTime adjustedEndTime = endDateTime.value_or(CommonUtils::Time::endOfDate(beginDateTime));

    if (shouldUpdateActiveDatesFrame)
        activeDatesFrame.updateDates(beginDateTime, adjustedEndTime);
    return getEventsBetweenDatesForCalendars(beginDateTime, adjustedEndTime);
}

void CalendarsManager::reloadEvents() const
{
    notify(getEventsBetweenDatesForCalendars(activeDatesFrame.beginDateTime, activeDatesFrame.endDateTime));
}

// Private methods

std::set<SharedCalendarEvent> CalendarsManager::getEventsBetweenDatesForCalendars(
    const DateTime beginDateTime,
    const DateTime endDateTime) const
{
    auto activeCalendars = getCalendars();

    std::set<SharedCalendarEvent> collectedEvents;
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
                                            const SharedCalendarEvent& event) {
            qDebug() << "Event name" << event->getName();
            qDebug() << "beginDateTime: " << CommonUtils::Time::stringFromStdChrono(beginDateTime);
            qDebug() << "endDateTime: " << CommonUtils::Time::stringFromStdChrono(endDateTime);

            qDebug() << "event->getBeginDateTime: "
                     << CommonUtils::Time::stringFromStdChrono(event->getBeginDateTime());
            if (event->getEndDateTime().has_value())
                qDebug() << "event->getEndDateTime: "
                         << CommonUtils::Time::stringFromStdChrono(event->getEndDateTime().value());

            if (event->isRangeEvent())
            {
                bool isPassedMultiDayBeginTime = CommonUtils::Time::isBetweenDates(
                    beginDateTime, event->getBeginDateTime(), event->getEndDateTime().value());

                bool isPassedMultiDayEndTime =
                    event->getEndDateTime().has_value() &&
                    CommonUtils::Time::isBetweenDates(
                        endDateTime, event->getBeginDateTime(), event->getEndDateTime().value());

                return isPassedMultiDayBeginTime && isPassedMultiDayEndTime &&
                    activeCategoryNames.contains(event->getCategory());
            }
            else
            {
                bool isPassedSingleDayBeginTime = event->getBeginDateTime() >= beginDateTime;

                bool isPassedSingleDayEndTime = !event->getEndDateTime().has_value() &&
                    CommonUtils::Time::isBetweenDates(event->getBeginDateTime(), beginDateTime, endDateTime);

                return isPassedSingleDayBeginTime && isPassedSingleDayEndTime &&
                    activeCategoryNames.contains(event->getCategory());
            }
        };

        auto result = activeActualEvents | std::ranges::views::filter(activeActualEventsFilter);
        collectedEvents.insert(result.begin(), result.end());
    }
    return collectedEvents;
}
