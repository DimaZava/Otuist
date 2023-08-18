#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include "../../ModelsLayer/CalendarEvent/CalendarEvent.h"

#include <QHBoxLayout>
#include <QWidget>
#include <memory>

class EventWidgetDelegate
{
public:
    virtual void removeEventButtonDidClick(const SharedCalendarEvent& event) = 0;
};

class EventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EventWidget(const SharedCalendarEvent& event, QWidget* parent = nullptr);
    ~EventWidget() override;
    EventWidgetDelegate* delegate; // take care of this raw point

private:
    SharedCalendarEvent event;
    std::unique_ptr<QHBoxLayout> mainLayout;
    void setupInitialState();

private slots:
    void removeButtonDidClick(bool checked) const;
};

#endif // EVENTWIDGET_H
