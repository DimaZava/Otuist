#include "EventsLayout.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

#include <optional>

EventsLayout::EventsLayout(
    const std::shared_ptr<CalendarsRepository>& calendarsRepository,
    ISubject<CalendarSelectionDTO>& calendarSubject,
    QWidget* parent)
    : QVBoxLayout(parent)
    , calendarsRepository(calendarsRepository)
    , tmp(new QTextEdit{"test"})
{
    calendarsRepository->addObserver(this);
    calendarSubject.addObserver(this);
    configureLayout();
}

EventsLayout::~EventsLayout()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void EventsLayout::configureLayout()
{
    tmp->setContentsMargins(InterfaceUtils::zeroMargins);
    tmp->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addWidget(tmp);
}

void EventsLayout::setCalendarEvents(const std::set<std::shared_ptr<CalendarEvent>>& calendarEvents)
{
    this->calendarEvents = calendarEvents;
    reloadData();
}

void EventsLayout::didChange(const std::set<std::shared_ptr<CalendarEvent>>& value)
{
    setCalendarEvents(value);
}

void EventsLayout::didChange(const CalendarSelectionDTO& value)
{
    std::optional<std::chrono::time_point<std::chrono::system_clock>> beginChronoDate;
    if (value.beginDate.has_value())
        beginChronoDate = CommonUtils::Time::stdChronoTimePointFromQDate(value.beginDate.value());

    std::optional<std::chrono::time_point<std::chrono::system_clock>> endChronoDate;
    if (value.endDate.has_value())
    {
        if (value.beginDate == value.endDate)
        {
            if (beginChronoDate.has_value())
                endChronoDate = CommonUtils::Time::endOfDate(beginChronoDate.value());
        }
        else
        {
            endChronoDate =
                CommonUtils::Time::endOfDate(CommonUtils::Time::stdChronoTimePointFromQDate(value.endDate.value()));
        }
    }

    setCalendarEvents(calendarsRepository->getEvents(beginChronoDate, endChronoDate));
}

void EventsLayout::reloadData()
{
    tmp->clear();
    for (const auto& event : calendarEvents)
    {
        std::string eventString;
        eventString += "Name: " + event->getName() + "\n";
        eventString += "CalendarName: " + event->getCalendarName().value_or("N/A") + "\n";
        eventString += "Begin Date: " + CommonUtils::Time::stringFromStdChrono(event->getBeginDateTime()) + "\n";
        if (event->getEndDateTime().has_value())
        {
            eventString +=
                "End Date: " + CommonUtils::Time::stringFromStdChrono(event->getEndDateTime().value()) + "\n";
        }
        eventString += "Description: " + event->getDescription().value_or("N/A") + "\n";
        eventString += "----------\n";

        auto qString = tmp->toPlainText();
        qString += eventString;
        tmp->setText(qString);
    }
}
