#ifndef CALENDARSMANAGER_H
#define CALENDARSMANAGER_H

#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../../BusinessLogicLayer/ObjectsRepository.h"
#include "../../BusinessLogicLayer/Observer.h"
#include "../../ModelsLayer/CalendarItem/CalendarItem.h"

struct ActiveDatesFrame
{
    std::chrono::time_point<std::chrono::system_clock> beginDateTime;
    std::chrono::time_point<std::chrono::system_clock> endDateTime;

    ActiveDatesFrame(
        std::chrono::time_point<std::chrono::system_clock> beginDateTime,
        std::chrono::time_point<std::chrono::system_clock> endDateTime)
        : beginDateTime(beginDateTime)
        , endDateTime(endDateTime)
    {}

    void updateDates(
        const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
        const std::chrono::time_point<std::chrono::system_clock> endDateTime)
    {
        this->beginDateTime = beginDateTime;
        this->endDateTime = endDateTime;
    }
};

class CalendarsManager
    : protected ObjectsRepository<std::shared_ptr<CalendarItem>>
    , public ISubject<std::set<std::shared_ptr<CalendarEvent>>>
{
public:
    CalendarsManager();
    CalendarsManager(const CalendarsManager& repository) = delete;
    CalendarsManager(CalendarsManager&& repository) = delete;
    ~CalendarsManager() override;

    void addCalendar(const std::shared_ptr<CalendarItem>& calendar);
    std::set<std::shared_ptr<CalendarItem>> getCalendars() const;
    std::optional<std::shared_ptr<CalendarItem>> getCalendar(const std::string& name) const;
    void updateCalendar(const std::string& name, const std::shared_ptr<CalendarItem>& calendar);
    void deleteCalendar(const std::string& name);
    void setCalendarsCategoryActive(const std::string& calendarName, const std::string& categoryName, bool isActive)
        const;

    void addEvent(const std::shared_ptr<CalendarEvent>& event) const;
    void removeEvent(const std::shared_ptr<CalendarEvent>& event) const;
    void removeEvents(const std::set<std::shared_ptr<CalendarEvent>>& events) const;
    std::set<std::shared_ptr<CalendarEvent>> getEvents(
        const std::chrono::time_point<std::chrono::system_clock>& beginDateTime,
        const std::optional<std::chrono::time_point<std::chrono::system_clock>>& endDateTime,
        bool shouldUpdateActiveDatesFrame = true);
    void reloadEvents() const;
    
    CalendarsManager& operator=(const CalendarsManager& other) = delete;
    CalendarsManager& operator=(CalendarsManager&& other) = delete;

private:
    std::set<std::shared_ptr<CalendarEvent>> getEventsBetweenDatesForCalendars(
        const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
        const std::chrono::time_point<std::chrono::system_clock> endDateTime) const;

    ActiveDatesFrame activeDatesFrame{CommonUtils::Time::beginOfDate(), CommonUtils::Time::endOfDate()};
};

#endif // CALENDARSMANAGER_H
