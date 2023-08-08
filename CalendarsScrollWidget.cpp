#include "CalendarsScrollWidget.h"

#include "InterfaceUtils.h"

CalendarsScrollWidget::CalendarsScrollWidget(QWidget* parent)
    : QScrollArea(parent)
    , calendarsLayoutContainer(std::unique_ptr<QWidget>{new QWidget{this}})
    , calendarsLayout(std::unique_ptr<CalendarsLayout>{new CalendarsLayout{calendarsLayoutContainer.get()}})
{
    configureLayout();
}

CalendarsScrollWidget::~CalendarsScrollWidget()
{
    calendarsLayoutContainer.release();
    calendarsLayout.release();
}

void CalendarsScrollWidget::configureLayout()
{
    calendarsLayoutContainer->setLayout(calendarsLayout.get());

    setWidgetResizable(true);
    setWidget(calendarsLayoutContainer.get());
    setMinimumWidth(200);
    setMaximumWidth(300);

    calendarsLayout->setSizeConstraint(QLayout::SetMaximumSize);
    calendarsLayout->setContentsMargins(InterfaceUtils::defaultMargins);
}

void CalendarsScrollWidget::setCalendarItems(const std::set<std::shared_ptr<CalendarItem>>& calendarItems)
{
    calendarsLayout->setCalendarItems(calendarItems);
}
