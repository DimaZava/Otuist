#include "EventsScrollWidget.h"

#include "InterfaceUtils.h"

EventsScrollWidget::EventsScrollWidget(Subject& subject, QWidget* parent)
    : QScrollArea(parent)
    , eventsLayoutContainer(std::make_unique<QWidget>(this))
    , eventsLayout(std::make_unique<EventsLayout>(subject, eventsLayoutContainer.get()))
{
    configureLayout();
}

EventsScrollWidget::~EventsScrollWidget()
{
    eventsLayoutContainer.reset();
    eventsLayout.reset();
}

void EventsScrollWidget::configureLayout()
{
    eventsLayoutContainer->setLayout(eventsLayout.get());

    setWidgetResizable(true);
    setWidget(eventsLayoutContainer.get());

    eventsLayout->setSizeConstraint(QLayout::SetMaximumSize);
    eventsLayout->setContentsMargins(InterfaceUtils::defaultMargins);
}
