#ifndef CALENDARSSCROLLWIDGET_H
#define CALENDARSSCROLLWIDGET_H

#include "../../BusinessLogicLayer/CalendarsRepository/CalendarsRepository.h"
#include "CalendarsLayout.h"

#include <QScrollArea>
#include <QVBoxLayout>

class CalendarsScrollWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit CalendarsScrollWidget(
        const std::shared_ptr<CalendarsRepository>& calendarsRepository,
        QWidget* parent = nullptr);
    ~CalendarsScrollWidget() override;

private:
    std::unique_ptr<QWidget> calendarsLayoutContainer;
    std::unique_ptr<CalendarsLayout> calendarsLayout;

    void configureLayout();
};

#endif // CALENDARSSCROLLWIDGET_H
