#include "EventWidget.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../AddEventDialog/AddEventDialog.h"
#include "../InterfaceUtils.h"

#include <QLabel>

EventWidget::EventWidget(
    const SharedCalendarEvent& event,
    const std::shared_ptr<CalendarsManager>& calendarsManager,
    QWidget* parent)
    : QWidget{parent}
    , event(event)
    , calendarsManager(calendarsManager)
    , mainLayout(std::make_unique<QHBoxLayout>(this))
    , editButton(std::make_unique<QPushButton>(this))
    , deleteButton(std::make_unique<QPushButton>(this))
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
    eventString += "Description: " + event->getDescription().value_or("N/A");
    eventLabel->setText(eventString.c_str());

    editButton->setText(tr("Edit"));
    editButton->setFixedWidth(120);
    connect(editButton.get(), &QPushButton::clicked, this, &EventWidget::editButtonDidClick);

    deleteButton->setText(tr("Remove"));
    deleteButton->setFixedWidth(120);
    connect(deleteButton.get(), &QPushButton::clicked, this, &EventWidget::removeButtonDidClick);

    mainLayout->addWidget(eventLabel);
    mainLayout->addWidget(editButton.get());
    mainLayout->addWidget(deleteButton.get());
}

void EventWidget::editButtonDidClick(bool checked)
{
    AddEventDialog dialog(event, calendarsManager->getCalendars(), this);
    if (dialog.exec() == QDialog::Accepted)
    {
        auto addEventDTO = dialog.getReturnValue();

        event->setName(addEventDTO.name);
        event->setCalendarName(addEventDTO.calendarName);
        event->setCategory(addEventDTO.category);
        event->setBeginDateTime(addEventDTO.beginDateTime);
        event->setEndDateTime(addEventDTO.endDateTime);
        event->setDescription(addEventDTO.description);

        delegate->eventUpdateDidFinish();
    }
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
