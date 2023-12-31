#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"

#include <QDate>
#include <optional>
#include <string>

class CalendarEvent
{
public:
    CalendarEvent(
        const std::string& name,
        const std::string& calendarName,
        const std::string& category,
        const DateTime& beginDateTime,
        const std::optional<DateTime>& endDateTime,
        const std::optional<std::string>& description);
    CalendarEvent(const CalendarEvent& event);
    CalendarEvent(CalendarEvent&& event) noexcept;
    ~CalendarEvent();

    CalendarEvent& operator=(const CalendarEvent& other);
    CalendarEvent& operator=(CalendarEvent&& other) noexcept;

    bool operator<(const CalendarEvent& other) const;
    bool operator==(const CalendarEvent& other) const;
    bool operator!=(const CalendarEvent& other) const;

    const DateTime getBeginDateTime() const;
    void setBeginDateTime(const DateTime& dateTime);

    const std::optional<DateTime> getEndDateTime() const;
    void setEndDateTime(const std::optional<DateTime>& dateTime);

    const std::string getName() const;
    void setName(const std::string& name);

    const std::optional<std::string> getDescription() const;
    void setDescription(const std::optional<std::string>& description);

    const std::optional<std::string> getColor() const;
    void setColor(const std::optional<std::string>& color);

    const std::string getCalendarName() const;
    void setCalendarName(const std::string& calendarName);

    const std::optional<std::string> getLocation() const;
    void setLocation(const std::optional<std::string>& location);

    const std::optional<std::string> getRRule() const;
    void setRRule(const std::optional<std::string>& rRule);

    const std::optional<std::string> getExdates() const;
    void setExdates(const std::optional<std::string>& exdates);

    const std::string getCategory() const;
    void setCategory(const std::string& category);

    const std::string getUid() const;

    const std::optional<std::string> getHref() const;
    void setHref(const std::optional<std::string>& href);

    bool isRangeEvent() const;

private:
    DateTime beginDateTime;
    std::optional<DateTime> endDateTime;
    std::string name;
    std::optional<std::string> description;
    std::optional<std::string> color;
    std::string calendarName;
    std::optional<std::string> location;
    std::optional<std::string> rRule;
    std::optional<std::string> exdates;
    std::string category;
    std::string uid;
    std::optional<std::string> href;
};

typedef std::shared_ptr<CalendarEvent> SharedCalendarEvent;

struct CalendarEventComparator
{
    bool operator()(const SharedCalendarEvent& left, const SharedCalendarEvent& right) const
    {
        return *left < *right;
    }
};

#endif // CALENDAREVENT_H
