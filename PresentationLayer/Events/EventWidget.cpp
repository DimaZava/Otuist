#include "EventWidget.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

#include <QLabel>
#include <QPushButton>

EventWidget::EventWidget(const std::shared_ptr<CalendarEvent>& event, QWidget* parent)
    : QWidget{parent}
    , event(event)
    , mainLayout(std::make_unique<QHBoxLayout>(this))
{
    setupInitialState();
}

EventWidget::~EventWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
    delegate = nullptr;
}

void EventWidget::setupInitialState()
{
    setLayout(mainLayout.get());

    QLabel* eventLabel = new QLabel;
    std::string eventString;
    eventString += "Name: " + event->getName() + "\n";
    eventString += "CalendarName: " + event->getCalendarName() + "\n";
    eventString += "Category: " + event->getCategory() + "\n";
    eventString += "Begin Date: " + CommonUtils::Time::stringFromStdChrono(event->getBeginDateTime()) + "\n";
    if (event->getEndDateTime().has_value())
    {
        eventString += "End Date: " + CommonUtils::Time::stringFromStdChrono(event->getEndDateTime().value()) + "\n";
    }
    eventString += "Description: " + event->getDescription().value_or("N/A") + "\n";

    eventLabel->setText(eventString.c_str());

    QPushButton* deleteButton = new QPushButton(this);
    deleteButton->setText(tr("Remove"));
    deleteButton->setFixedWidth(120);

    connect(deleteButton, &QPushButton::clicked, this, &EventWidget::removeButtonDidClick);

    mainLayout->addWidget(eventLabel);
    mainLayout->addWidget(deleteButton);
}

void EventWidget::removeButtonDidClick(bool checked) const
{
    const int ret = InterfaceUtils::showConfirmationAlert(
        tr("Delete Event"),
        tr("Do you really want to remove event %1?").arg(QString(event->getName().c_str())),
        QMessageBox::Ok | QMessageBox::Cancel,
        QMessageBox::Ok);

    switch (ret)
    {
        case QMessageBox::Ok:
            qDebug() << "Ok";
            delegate->removeEventButtonDidClick(event);
            break;
        case QMessageBox::Cancel:
            break;
        default:
            static_assert(true, "Unexpected condition");
    }
}
