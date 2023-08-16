#include "CalendarsScrollWidget.h"

#include "../InterfaceUtils.h"

CalendarsScrollWidget::CalendarsScrollWidget(
    const std::shared_ptr<CalendarsRepository>& calendarsRepository,
    QWidget* parent)
    : QScrollArea(parent)
    , calendarsLayoutContainer(std::make_unique<QWidget>(this))
    , calendarsLayout(std::make_unique<CalendarsLayout>(calendarsRepository, calendarsLayoutContainer.get()))
{
    configureLayout();
}

CalendarsScrollWidget::~CalendarsScrollWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
    calendarsLayoutContainer.reset();
    calendarsLayout.reset();
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
