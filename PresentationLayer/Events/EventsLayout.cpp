#include "EventsLayout.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

#include <QLabel>

EventsLayout::EventsLayout(
    const std::shared_ptr<CalendarsRepository>& calendarsRepository,
    ISubject<CalendarSelectionDTO>& calendarSubject,
    QWidget* parent)
    : QVBoxLayout(parent)
    , eventsList(std::make_unique<QListWidget>())
    , calendarsRepository(calendarsRepository)
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
    eventsList->setContentsMargins(InterfaceUtils::zeroMargins);
    eventsList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addWidget(eventsList.get());
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
    std::chrono::time_point<std::chrono::system_clock> beginChronoDate =
        CommonUtils::Time::stdChronoTimePointFromQDate(value.beginDate);

    std::chrono::time_point<std::chrono::system_clock> endChronoDate;
    if (value.beginDate == value.endDate)
    {
        endChronoDate = CommonUtils::Time::endOfDate(beginChronoDate);
    }
    else
    {
        endChronoDate = CommonUtils::Time::endOfDate(CommonUtils::Time::stdChronoTimePointFromQDate(value.endDate));
    }

    setCalendarEvents(calendarsRepository->getEvents(beginChronoDate, endChronoDate));
}

void EventsLayout::reloadData()
{
    QListWidgetItem* item;
    while ((item = eventsList->item(0)) != nullptr)
    {
        delete item;
    }

    for (const auto& event : calendarEvents)
    {
        QListWidgetItem* newItem = new QListWidgetItem;
        QLabel* eventLabel = new QLabel;
        std::string eventString;
        eventString += "Name: " + event->getName() + "\n";
        eventString += "CalendarName: " + event->getCalendarName() + "\n";
        eventString += "Category: " + event->getCategory() + "\n";
        eventString += "Begin Date: " + CommonUtils::Time::stringFromStdChrono(event->getBeginDateTime()) + "\n";
        if (event->getEndDateTime().has_value())
        {
            eventString +=
                "End Date: " + CommonUtils::Time::stringFromStdChrono(event->getEndDateTime().value()) + "\n";
        }
        eventString += "Description: " + event->getDescription().value_or("N/A") + "\n";
        eventString += "----------\n";

        // tmp->setContentsMargins(InterfaceUtils::zeroMargins);
        // tmp->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        eventLabel->setText(eventString.c_str());
        // eventLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

        newItem->setSizeHint(eventLabel->sizeHint());
        eventsList->addItem(newItem);
        eventsList->setItemWidget(newItem, eventLabel);
    }
}
