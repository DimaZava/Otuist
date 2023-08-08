#include "Event.h"

Event::Event(
    const std::chrono::time_point<std::chrono::system_clock>& beginDateTime,
    const std::optional<std::chrono::time_point<std::chrono::system_clock>>& endDateTime,
    const std::string& name,
    const std::optional<std::string>& description)
    : beginDateTime(beginDateTime)
    , endDateTime(endDateTime)
    , name(name)
    , description(description)
{}

const std::chrono::time_point<std::chrono::system_clock> Event::getBeginDateTime() const
{
    return beginDateTime;
}

void Event::setBeginDateTime(const std::chrono::time_point<std::chrono::system_clock>& dateTime)
{
    this->beginDateTime = dateTime;
}

const std::optional<std::chrono::time_point<std::chrono::system_clock>> Event::getEndDateTime() const
{
    return endDateTime;
}

void Event::endEndDateTime(const std::optional<std::chrono::time_point<std::chrono::system_clock>>& dateTime)
{
    this->endDateTime = dateTime;
}

const std::string Event::getName() const
{
    return name;
}

void Event::setName(const std::string& name)
{
    this->name = name;
}

const std::optional<std::string> Event::getDescription() const
{
    return description;
}

void Event::setDescription(const std::optional<std::string>& description)
{
    this->description = description;
}

const std::optional<std::string> Event::getColor() const
{
    return color;
}

void Event::setColor(const std::optional<std::string>& color)
{
    this->color = color;
}

const std::optional<std::string> Event::getCalendarName() const
{
    return calendarName;
}

void Event::setCalendarName(const std::optional<std::string>& calendarName)
{
    this->calendarName = calendarName;
}

const std::optional<std::string> Event::getLocation() const
{
    return location;
}

void Event::setLocation(const std::optional<std::string>& location)
{
    this->location = location;
}

const std::optional<std::string> Event::getRRule() const
{
    return rRule;
}

void Event::setRRule(const std::optional<std::string>& rRule)
{
    this->rRule = rRule;
}

const std::optional<std::string> Event::getExdates() const
{
    return exdates;
}

void Event::setExdates(const std::optional<std::string>& exdates)
{
    this->exdates = exdates;
}

const std::optional<std::string> Event::getCategory() const
{
    return categories;
}

void Event::setCategories(const std::optional<std::string>& categories)
{
    this->categories = categories;
}

const std::optional<std::string> Event::getUid() const
{
    return uid;
}

void Event::setUid(const std::optional<std::string>& uid)
{
    this->uid = uid;
}

const std::optional<std::string> Event::getHref() const
{
    return href;
}

void Event::setHref(const std::optional<std::string>& href)
{
    this->href = href;
}
