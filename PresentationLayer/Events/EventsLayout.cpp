#include "EventsLayout.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

EventsLayout::EventsLayout(
    const std::shared_ptr<CalendarsManager>& calendarsManager,
    ISubject<CalendarSelectionDTO>& calendarSubject,
    QWidget* parent)
    : QVBoxLayout(parent)
    , eventsList(std::make_unique<QListWidget>())
    , calendarsManager(calendarsManager)
{
    calendarsManager->addObserver(this);
    calendarSubject.addObserver(this);
    configureLayout();
}

EventsLayout::~EventsLayout()
{
    qDebug() << __PRETTY_FUNCTION__;
    calendarsManager->removeObserver(this);
    cleanItems();
}

void EventsLayout::configureLayout()
{
    eventsList->setContentsMargins(InterfaceUtils::zeroMargins);
    eventsList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addWidget(eventsList.get());
}

void EventsLayout::setCalendarEvents(const std::set<SharedCalendarEvent>& calendarEvents)
{
    // Have to transform since this instance operates with custom sorted events
    // TODO: Maybe sort it everywhere?
    this->calendarEvents.clear();
    std::transform(
        calendarEvents.begin(),
        calendarEvents.end(),
        std::inserter(this->calendarEvents, this->calendarEvents.end()),
        [](const SharedCalendarEvent& event) {
        return event;
        });
    reloadData();
}

void EventsLayout::didChange(const std::set<SharedCalendarEvent>& value)
{
    setCalendarEvents(value);
}

void EventsLayout::didChange(const CalendarSelectionDTO& value)
{
    DateTime beginChronoDate =
        CommonUtils::Time::stdChronoTimePointFromQDate(value.beginDate);

    std::optional<DateTime> endChronoDate;
    if (value.beginDate != value.endDate)
    {
        endChronoDate = CommonUtils::Time::endOfDate(CommonUtils::Time::stdChronoTimePointFromQDate(value.endDate));
    }

    setCalendarEvents(calendarsManager->getEvents(beginChronoDate, endChronoDate));
}

void EventsLayout::reloadData()
{
    cleanItems();

    for (const auto& event : calendarEvents)
    {
        QListWidgetItem* newItem = new QListWidgetItem;
        EventWidget* eventWidget = new EventWidget{event, calendarsManager};

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

void EventsLayout::removeEventButtonDidClick(const SharedCalendarEvent& event)
{
    calendarsManager->removeEvent(event);
}

void EventsLayout::eventUpdateDidFinish()
{
    calendarsManager->reloadEvents();
}
