#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include "../../BusinessLogicLayer/CalendarsManager/CalendarsManager.h"
#include "../../BusinessLogicLayer/Observer.h"

#include <QCalendarWidget>
#include <QPainter>
#include <QTextCharFormat>
#include <optional>

struct CalendarSelectionDTO
{
    QDate beginDate;
    QDate endDate;
};

class CalendarWidget
    : public QCalendarWidget
    , public ISubject<CalendarSelectionDTO>
    , public IObserver<std::set<SharedCalendarEvent>>
{
    Q_OBJECT
public:
    explicit CalendarWidget(const std::shared_ptr<CalendarsManager>& calendarsManager);
    ~CalendarWidget() override;
    void didChange(const std::set<SharedCalendarEvent>& value) override;

protected:
    void paintCell(QPainter* painter, const QRect& rect, QDate date) const override;

private:
    std::optional<QDate> beginDate;
    std::optional<QDate> endDate;
    std::unique_ptr<QTextCharFormat> highlighter;
    int currentMonth;
    const std::shared_ptr<CalendarsManager>& calendarsManager;

    void performInitialSetup();
    void highlightRange(QTextCharFormat format);
    std::optional<QDate> dateFromPosition(const QPoint& position);

    static const QPen bordersPen();
    static const QPen datePen(QPen bordersPen);
    static const QPen nonCurrentMonthDatePen();
    static const QPen eventsCounterPen();
    static const QBrush todayIndicatorBrush();

private slots:
    void selectDateRange(QDate date);
    void provideContextMenu(const QPoint& pos);
    void currentPageDidChange(int year, int month);
};

#endif // CALENDARWIDGET_H
