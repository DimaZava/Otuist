#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include "../../BusinessLogicLayer/CalendarsManager/CalendarsManager.h"
#include "../../ModelsLayer/CalendarEvent/CalendarEvent.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <memory>

class EventWidgetDelegate
{
public:
    virtual void removeEventButtonDidClick(const SharedCalendarEvent& event) = 0;
    virtual void eventUpdateDidFinish() = 0;
};

class EventWidget : public QWidget
{
    Q_OBJECT
public:
    EventWidget(
        const SharedCalendarEvent& event,
        const std::shared_ptr<CalendarsManager>& calendarsManager,
        QWidget* parent = nullptr);
    ~EventWidget() override;
    EventWidgetDelegate* delegate; // take care of this raw point

private:
    SharedCalendarEvent event;
    const std::shared_ptr<CalendarsManager>& calendarsManager;

    std::unique_ptr<QHBoxLayout> mainLayout;
    std::unique_ptr<QPushButton> editButton;
    std::unique_ptr<QPushButton> deleteButton;

    void setupInitialState();

private slots:
    void editButtonDidClick(bool checked);
    void removeButtonDidClick(bool checked) const;
};

#endif // EVENTWIDGET_H
