#include "EventsLayout.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

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
    calendarsRepository->removeObserver(this);
    cleanItems();
}

void EventsLayout::configureLayout()
{
    eventsList->setContentsMargins(InterfaceUtils::zeroMargins);
    eventsList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addWidget(eventsList.get());
}

void EventsLayout::setCalendarEvents(const std::set<std::shared_ptr<CalendarEvent>>& calendarEvents)
{
    // Have to transform since this instance operates with custom sorted events
    // TODO: Maybe sort it everywhere?
    this->calendarEvents.clear();
    std::transform(
        calendarEvents.begin(),
        calendarEvents.end(),
        std::inserter(this->calendarEvents, this->calendarEvents.end()),
        [](const std::shared_ptr<CalendarEvent>& event) {
        return event;
        });
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

    std::optional<std::chrono::time_point<std::chrono::system_clock>> endChronoDate;
    if (value.beginDate != value.endDate)
    {
        endChronoDate = CommonUtils::Time::endOfDate(CommonUtils::Time::stdChronoTimePointFromQDate(value.endDate));
    }

    setCalendarEvents(calendarsRepository->getEvents(beginChronoDate, endChronoDate));
}

void EventsLayout::reloadData()
{
    cleanItems();

    for (const auto& event : calendarEvents)
    {
        QListWidgetItem* newItem = new QListWidgetItem;
        EventWidget* eventWidget = new EventWidget{event};

        eventWidget->delegate = this;

        newItem->setSizeHint(eventWidget->sizeHint());
        eventsList->addItem(newItem);
        eventsList->setItemWidget(newItem, eventWidget);
    }
}

void EventsLayout::cleanItems()
{
    QListWidgetItem* item;
    while ((item = eventsList->item(0)) != nullptr)
    {
        EventWidget* eventWidget = dynamic_cast<EventWidget*>(eventsList->itemWidget(item));
        eventWidget->delegate = nullptr;
        delete item;
    }
}

// EventsWidgetDelegate

void EventsLayout::removeEventButtonDidClick(const std::shared_ptr<CalendarEvent>& event)
{
    calendarsRepository->removeEvent(event);
}
