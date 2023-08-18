#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <set>
#include <string>

#include "../CalendarEvent/CalendarEvent.h"
#include "../NamedObjectInterface.h"

class CalendarCategory
{
public:
    const std::string name;
    bool isActive = true;

    explicit CalendarCategory(const std::string& name)
        : name(name)
    {}

    bool operator<(const CalendarCategory& other) const
    {
        return this->name.compare(other.name) < 0;
    }

    bool operator==(const CalendarCategory& other) const
    {
        return !(*this < other) && !(other < *this);
    }

    bool operator!=(const CalendarCategory& other) const
    {
        return *this < other || other < *this;
    }
};

struct CalendarCategoryComparator
{
    bool operator()(const std::shared_ptr<CalendarCategory>& left, const std::shared_ptr<CalendarCategory>& right) const
    {
        return *left < *right;
    }
};

class CalendarItem : NamedObjectInterface
{
public:
    CalendarItem(
        const std::string& name,
        const std::set<std::shared_ptr<CalendarCategory>, CalendarCategoryComparator>& categories,
        const std::set<std::shared_ptr<CalendarEvent>>& events = {});
    CalendarItem(const CalendarItem& calendar);
    CalendarItem(CalendarItem&& calendar) noexcept;
    ~CalendarItem() override;

    std::string getName() const override;
    std::set<std::shared_ptr<CalendarCategory>, CalendarCategoryComparator> getCategories() const;
    void addEvent(const std::shared_ptr<CalendarEvent>& event);
    void removeEvent(const std::shared_ptr<CalendarEvent>& event);
    std::set<std::shared_ptr<CalendarEvent>> getEvents() const;

    CalendarItem& operator=(const CalendarItem& other);
    CalendarItem& operator=(CalendarItem&& other) noexcept;

    bool operator<(const CalendarItem& other) const;
    bool operator==(const CalendarItem& other) const;
    bool operator!=(const CalendarItem& other) const;

private:
    std::string name;
    std::set<std::shared_ptr<CalendarCategory>, CalendarCategoryComparator> categories;
    std::set<std::shared_ptr<CalendarEvent>> events;
};

#endif // CALENDARITEM_H
