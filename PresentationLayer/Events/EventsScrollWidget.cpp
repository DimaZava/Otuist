#include "EventsScrollWidget.h"

#include "../InterfaceUtils.h"

EventsScrollWidget::EventsScrollWidget(
    const std::shared_ptr<CalendarsManager>& calendarsManager,
    ISubject<CalendarSelectionDTO>& calendarSubject,
    QWidget* parent)
    : QScrollArea(parent)
    , eventsLayoutContainer(std::make_unique<QWidget>(this))
    , eventsLayout(std::make_unique<EventsLayout>(calendarsManager, calendarSubject, eventsLayoutContainer.get()))
{
    configureLayout();
}

EventsScrollWidget::~EventsScrollWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void EventsScrollWidget::configureLayout()
{
    eventsLayoutContainer->setLayout(eventsLayout.get());

    setWidgetResizable(true);
    setWidget(eventsLayoutContainer.get());

    eventsLayout->setSizeConstraint(QLayout::SetMaximumSize);
    eventsLayout->setContentsMargins(InterfaceUtils::zeroMargins);
}
