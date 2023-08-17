#ifndef CALENDARSREPOSITORY_H
#define CALENDARSREPOSITORY_H

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

class CalendarsRepository
    : protected ObjectsRepository<std::shared_ptr<CalendarItem>>
    , public ISubject<std::set<std::shared_ptr<CalendarEvent>>>
{
public:
    CalendarsRepository();
    CalendarsRepository(const CalendarsRepository& repository) = delete;
    CalendarsRepository(CalendarsRepository&& repository) = delete;
    ~CalendarsRepository() override;

    void addCalendar(const std::shared_ptr<CalendarItem>& calendar);
    std::set<std::shared_ptr<CalendarItem>> getCalendars() const;
    std::optional<std::shared_ptr<CalendarItem>> getCalendar(const std::string& name) const;
    void updateCalendar(const std::string& name, const std::shared_ptr<CalendarItem>& calendar);
    void deleteCalendar(const std::string& name);
    void setCalendarsCategoryActive(const std::string& calendarName, const std::string& categoryName, bool isActive)
        const;

    void addEvent(const std::shared_ptr<CalendarEvent>& event);
    std::set<std::shared_ptr<CalendarEvent>> getEvents(
        const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
        const std::chrono::time_point<std::chrono::system_clock> endDateTime);
    void reloadEvents() const;

    CalendarsRepository& operator=(const CalendarsRepository& other) = delete;
    CalendarsRepository& operator=(CalendarsRepository&& other) = delete;

private:
    std::set<std::shared_ptr<CalendarEvent>> getEventsBetweenDatesForCalendars(
        const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
        const std::chrono::time_point<std::chrono::system_clock> endDateTime) const;

    ActiveDatesFrame activeDatesFrame{CommonUtils::Time::beginOfDate(), CommonUtils::Time::endOfDate()};
};

#endif // CALENDARSREPOSITORY_H
