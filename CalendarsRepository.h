#ifndef CALENDARSREPOSITORY_H
#define CALENDARSREPOSITORY_H

#include "CalendarItem.h"
#include "ObjectsRepository.h"
#include "Observer.h"

class CalendarsRepository
    : protected ObjectsRepository<std::shared_ptr<CalendarItem>>
    , public Subject<std::set<std::shared_ptr<CalendarEvent>>>
{
public:
    CalendarsRepository();
    // CalendarsRepository(const CalendarsRepository& repository) = delete;
    // CalendarsRepository(CalendarsRepository&& repository) = delete;
    ~CalendarsRepository() override;

    void addCalendar(const std::shared_ptr<CalendarItem>& calendar);
    std::set<std::shared_ptr<CalendarItem>> getCalendars() const;
    std::optional<std::shared_ptr<CalendarItem>> getCalendar(const std::string& name) const;
    void updateCalendar(const std::string& name, const std::shared_ptr<CalendarItem>& calendar);
    void deleteCalendar(const std::string& name);

    CalendarsRepository& operator=(const CalendarsRepository& other) = delete;
    CalendarsRepository& operator=(CalendarsRepository&& other) = delete;

private:
    std::set<std::shared_ptr<CalendarEvent>> getEventsBetweenDatesForCalendars(
        const std::chrono::time_point<std::chrono::system_clock> beginDateTime,
        const std::optional<std::chrono::time_point<std::chrono::system_clock>> = std::nullopt,
        const std::optional<std::set<std::string>> calendarNames = std::nullopt) const;
};

#endif // CALENDARSREPOSITORY_H
