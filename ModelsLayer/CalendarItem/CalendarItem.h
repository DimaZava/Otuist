#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <set>
#include <string>

#include "../CalendarEvent/CalendarEvent.h"
#include "../NamedObjectInterface.h"

struct CalendarCategory
{
    const std::string name;
    bool isActive = true;

    explicit CalendarCategory(const std::string& name)
        : name(name)
    {}
};

class CalendarItem : NamedObjectInterface
{
public:
    CalendarItem(
        const std::string& name,
        const std::set<std::shared_ptr<CalendarCategory>>& categories,
        const std::set<std::shared_ptr<CalendarEvent>>& events = {});
    CalendarItem(const CalendarItem& calendar);
    CalendarItem(CalendarItem&& calendar) noexcept;
    ~CalendarItem() override;

    std::string getName() const override;
    std::set<std::shared_ptr<CalendarCategory>> getCategories() const;
    std::set<std::shared_ptr<CalendarEvent>> getEvents() const;

    std::string getUuid() const;

    CalendarItem& operator=(const CalendarItem& other);
    CalendarItem& operator=(CalendarItem&& other) noexcept;

    bool operator<(const CalendarItem& other) const;
    bool operator==(const CalendarItem& other) const;
    bool operator!=(const CalendarItem& other) const;

private:
    std::string uuid;
    std::string name;
    std::set<std::shared_ptr<CalendarCategory>> categories;
    std::set<std::shared_ptr<CalendarEvent>> events;
};

#endif // CALENDARITEM_H
