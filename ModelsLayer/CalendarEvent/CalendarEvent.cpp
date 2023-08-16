#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(
    const std::string& name,
    const std::string& calendarName,
    const std::string& category,
    const std::chrono::time_point<std::chrono::system_clock>& beginDateTime,
    const std::optional<std::chrono::time_point<std::chrono::system_clock>>& endDateTime,
    const std::optional<std::string>& description)
    : name(name)
    , calendarName(calendarName)
    , category(category)
    , beginDateTime(beginDateTime)
    , endDateTime(endDateTime)
    , description(description)
{}

CalendarEvent::CalendarEvent(const CalendarEvent& event)
    : name(event.name)
    , calendarName(event.calendarName)
    , category(event.category)
    , beginDateTime(event.beginDateTime)
    , endDateTime(event.endDateTime)
    , description(event.description)
{}

CalendarEvent::CalendarEvent(CalendarEvent&& event) noexcept
    : name(std::exchange(event.name, nullptr))
    , calendarName(std::exchange(event.calendarName, nullptr))
    , category(std::exchange(event.category, nullptr))
    , beginDateTime(event.beginDateTime) // trivially-copyable type
    , endDateTime(event.endDateTime)     // trivially-copyable type
    , description(std::exchange(event.description, nullptr))
{}

CalendarEvent::~CalendarEvent()
{
    qDebug() << __PRETTY_FUNCTION__;
}

CalendarEvent& CalendarEvent::operator=(const CalendarEvent& other)
{
    if (this != &other)
    {
        beginDateTime = other.beginDateTime;
        endDateTime = other.endDateTime;
        name = other.name;
        description = other.description;
    }
    return *this;
}

CalendarEvent& CalendarEvent::operator=(CalendarEvent&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    beginDateTime = other.beginDateTime; // trivially-copyable type
    endDateTime = other.endDateTime;     // trivially-copyable type
    name = std::exchange(other.name, nullptr);
    description = std::exchange(other.description, nullptr);

    return *this;
}

bool CalendarEvent::operator<(const CalendarEvent& other) const
{
    return this->name < other.name;
}

bool CalendarEvent::operator==(const CalendarEvent& other) const
{
    return !(*this < other) && !(other < *this);
}

bool CalendarEvent::operator!=(const CalendarEvent& other) const
{
    return *this < other || other < *this;
}

const std::chrono::time_point<std::chrono::system_clock> CalendarEvent::getBeginDateTime() const
{
    return beginDateTime;
}

void CalendarEvent::setBeginDateTime(const std::chrono::time_point<std::chrono::system_clock>& dateTime)
{
    this->beginDateTime = dateTime;
}

const std::optional<std::chrono::time_point<std::chrono::system_clock>> CalendarEvent::getEndDateTime() const
{
    return endDateTime;
}

void CalendarEvent::endEndDateTime(const std::optional<std::chrono::time_point<std::chrono::system_clock>>& dateTime)
{
    this->endDateTime = dateTime;
}

const std::string CalendarEvent::getName() const
{
    return name;
}

void CalendarEvent::setName(const std::string& name)
{
    this->name = name;
}

const std::optional<std::string> CalendarEvent::getDescription() const
{
    return description;
}

void CalendarEvent::setDescription(const std::optional<std::string>& description)
{
    this->description = description;
}

const std::optional<std::string> CalendarEvent::getColor() const
{
    return color;
}

void CalendarEvent::setColor(const std::optional<std::string>& color)
{
    this->color = color;
}

const std::string CalendarEvent::getCalendarName() const
{
    return calendarName;
}

void CalendarEvent::setCalendarName(const std::string& calendarName)
{
    this->calendarName = calendarName;
}

const std::optional<std::string> CalendarEvent::getLocation() const
{
    return location;
}

void CalendarEvent::setLocation(const std::optional<std::string>& location)
{
    this->location = location;
}

const std::optional<std::string> CalendarEvent::getRRule() const
{
    return rRule;
}

void CalendarEvent::setRRule(const std::optional<std::string>& rRule)
{
    this->rRule = rRule;
}

const std::optional<std::string> CalendarEvent::getExdates() const
{
    return exdates;
}

void CalendarEvent::setExdates(const std::optional<std::string>& exdates)
{
    this->exdates = exdates;
}

const std::string CalendarEvent::getCategory() const
{
    return category;
}

void CalendarEvent::setCategory(const std::string& category)
{
    this->category = category;
}

const std::optional<std::string> CalendarEvent::getUid() const
{
    return uid;
}

void CalendarEvent::setUid(const std::optional<std::string>& uid)
{
    this->uid = uid;
}

const std::optional<std::string> CalendarEvent::getHref() const
{
    return href;
}

void CalendarEvent::setHref(const std::optional<std::string>& href)
{
    this->href = href;
}
