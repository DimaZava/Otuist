#include "CalendarItem.h"

CalendarItem::CalendarItem(std::string name, std::set<std::string> categories, std::vector<Event> events)
    : name(name)
    , categories(categories)
    , events(events)
{}

CalendarItem::~CalendarItem() {}

std::string CalendarItem::getName() const
{
    return name;
}

std::vector<Event> CalendarItem::getEvents() const
{
    return events;
}

std::set<std::string> CalendarItem::getCategories() const
{
    return categories;
}
