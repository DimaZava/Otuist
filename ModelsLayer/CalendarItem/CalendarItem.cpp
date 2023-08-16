#include "CalendarItem.h"

CalendarItem::CalendarItem(
    const std::string& name,
    const std::set<std::string>& categories,
    const std::set<std::shared_ptr<CalendarEvent>>& events)
    : uuid(std::to_string(std::rand()))
    , name(name)
    , categories(categories)
    , events(events)
{
}

CalendarItem::CalendarItem(const CalendarItem& calendar)
    : uuid(calendar.uuid)
    , name(calendar.name)
    , categories(calendar.categories)
    , events(calendar.events)
{
}

CalendarItem::CalendarItem(CalendarItem&& calendar) noexcept
    : uuid(std::exchange(calendar.uuid, nullptr))
    , name(std::exchange(calendar.name, nullptr))
{
    categories = std::move(calendar.categories);
    calendar.categories.clear();
    events = std::move(calendar.events);
    calendar.events.clear();
}

CalendarItem::~CalendarItem()
{
    qDebug() << __PRETTY_FUNCTION__;
}

std::string CalendarItem::getUuid() const
{
    return uuid;
}

std::string CalendarItem::getName() const
{
    return name;
}

std::set<std::shared_ptr<CalendarEvent>> CalendarItem::getEvents() const
{
    return events;
}

std::set<std::string> CalendarItem::getCategories() const
{
    return categories;
}

CalendarItem& CalendarItem::operator=(const CalendarItem& other)
{
    uuid = other.uuid;
    name = other.name;
    categories = other.categories;
    events = other.events;

    return *this;
}

CalendarItem& CalendarItem::operator=(CalendarItem&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    uuid = std::exchange(other.uuid, nullptr);
    name = std::exchange(other.name, nullptr);

    categories = std::move(other.categories);
    other.categories.clear();
    events = std::move(other.events);
    other.events.clear();

    return *this;
}
