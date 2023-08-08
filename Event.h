#ifndef EVENT_H
#define EVENT_H

#include <QDate>
#include <optional>
#include <string>

class Event
{
public:
    Event(
        const std::chrono::time_point<std::chrono::system_clock>& beginDateTime,
        const std::optional<std::chrono::time_point<std::chrono::system_clock>>& endDateTime,
        const std::string& name,
        const std::optional<std::string>& description);

public slots:
    const std::chrono::time_point<std::chrono::system_clock> getBeginDateTime() const;
    void setBeginDateTime(const std::chrono::time_point<std::chrono::system_clock>& dateTime);

    const std::optional<std::chrono::time_point<std::chrono::system_clock>> getEndDateTime() const;
    void endEndDateTime(const std::optional<std::chrono::time_point<std::chrono::system_clock>>& dateTime);

    const std::string getName() const;
    void setName(const std::string& name);

    const std::optional<std::string> getDescription() const;
    void setDescription(const std::optional<std::string>& description);

    const std::optional<std::string> getColor() const;
    void setColor(const std::optional<std::string>& color);

    const std::optional<std::string> getCalendarName() const;
    void setCalendarName(const std::optional<std::string>& calendarName);

    const std::optional<std::string> getLocation() const;
    void setLocation(const std::optional<std::string>& location);

    const std::optional<std::string> getRRule() const;
    void setRRule(const std::optional<std::string>& rRule);

    const std::optional<std::string> getExdates() const;
    void setExdates(const std::optional<std::string>& exdates);
    
    const std::optional<std::string> getCategory() const;
    void setCategories(const std::optional<std::string>& categories);

    const std::optional<std::string> getUid() const;
    void setUid(const std::optional<std::string>& uid);

    const std::optional<std::string> getHref() const;
    void setHref(const std::optional<std::string>& href);

private:
    std::chrono::time_point<std::chrono::system_clock> beginDateTime;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime;
    std::string name;
    std::optional<std::string> description;
    std::optional<std::string> color;
    std::optional<std::string> calendarName;
    std::optional<std::string> location;
    std::optional<std::string> rRule;
    std::optional<std::string> exdates;
    std::optional<std::string> categories;
    std::optional<std::string> uid;
    std::optional<std::string> href;
};

#endif // EVENT_H
