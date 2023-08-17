#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include "../../ModelsLayer/CalendarEvent/CalendarEvent.h"

#include <QHBoxLayout>
#include <QWidget>
#include <memory>

class EventWidgetDelegate
{
public:
    virtual void removeEventButtonDidClick(const std::shared_ptr<CalendarEvent>& event) = 0;
};

class EventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EventWidget(const std::shared_ptr<CalendarEvent>& event, QWidget* parent = nullptr);
    ~EventWidget() override;
    EventWidgetDelegate* delegate; // take care of this raw point

private:
    std::shared_ptr<CalendarEvent> event;
    std::unique_ptr<QHBoxLayout> mainLayout;
    void setupInitialState();

private slots:
    void removeButtonDidClick(bool checked) const;
};

#endif // EVENTWIDGET_H
