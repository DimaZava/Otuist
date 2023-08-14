#include "EventsLayout.h"
#include "InterfaceUtils.h"

EventsLayout::EventsLayout(Subject& subject, QWidget* parent)
    : QVBoxLayout(parent)
    , tmp(new QTextEdit{"test"})
{
    subject.attach(*this);
    configureLayout();
}

EventsLayout::~EventsLayout() {}

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

void EventsLayout::reloadData()
{
    tmp->clear();
    for (const auto& event : calendarEvents)
    {
        std::string eventString;
        eventString += "Name: " + event->getName() + "\n";
        eventString += "CalendarName: " + event->getCalendarName().value_or("N/A") + "\n";
        eventString += "Description: " + event->getDescription().value_or("N/A") + "\n";
        eventString += "----------\n";

        auto qString = tmp->toPlainText();
        qString += eventString;
        tmp->setText(qString);
    }
}

void EventsLayout::update(Subject& subject)
{
    std::cout << "Observer received update. New state \n";
}
