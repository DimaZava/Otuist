#ifndef CALENDARSMANAGER_H
#define CALENDARSMANAGER_H

#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../../BusinessLogicLayer/NotificationManager/NotificationManager.h"
#include "../../BusinessLogicLayer/ObjectsRepository.h"
#include "../../BusinessLogicLayer/Observer.h"
#include "../../ModelsLayer/CalendarItem/CalendarItem.h"

struct ActiveDatesFrame
{
    DateTime beginDateTime;
    DateTime endDateTime;

    ActiveDatesFrame(DateTime beginDateTime, DateTime endDateTime)
        : beginDateTime(beginDateTime)
        , endDateTime(endDateTime)
    {}

    void updateDates(const DateTime beginDateTime, const DateTime endDateTime)
    {
        this->beginDateTime = beginDateTime;
        this->endDateTime = endDateTime;
    }
};

class CalendarsManager
    : protected ObjectsRepository<SharedCalendarItem>
    , public ISubject<std::set<SharedCalendarEvent>>
{
public:
    CalendarsManager();
    CalendarsManager(const CalendarsManager& repository) = delete;
    CalendarsManager(CalendarsManager&& repository) = delete;
    ~CalendarsManager() override;

    void addCalendar(const SharedCalendarItem& calendar);
    std::set<SharedCalendarItem> getCalendars() const;
    std::optional<SharedCalendarItem> getCalendar(const std::string& name) const;
    void updateCalendar(const std::string& name, const SharedCalendarItem& calendar);
    void deleteCalendar(const std::string& name);
    void setCalendarsCategoryActive(const std::string& calendarName, const std::string& categoryName, bool isActive)
        const;

    void addEvent(const SharedCalendarEvent& event) const;
    void removeEvent(const SharedCalendarEvent& event) const;
    void removeEvents(const std::set<SharedCalendarEvent>& events) const;
    std::set<SharedCalendarEvent> getEvents(
        const DateTime& beginDateTime,
        const std::optional<DateTime>& endDateTime,
        bool shouldUpdateActiveDatesFrame = true);
    void reloadEvents() const;
    
    CalendarsManager& operator=(const CalendarsManager& other) = delete;
    CalendarsManager& operator=(CalendarsManager&& other) = delete;

private:
    std::unique_ptr<NotificationManager> notificationsManager;

    std::set<SharedCalendarEvent> getEventsBetweenDatesForCalendars(
        const DateTime beginDateTime,
        const DateTime endDateTime) const;

    ActiveDatesFrame activeDatesFrame{CommonUtils::Time::beginOfDate(), CommonUtils::Time::endOfDate()};
};

#endif // CALENDARSMANAGER_H
