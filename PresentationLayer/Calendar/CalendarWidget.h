#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include "../../BusinessLogicLayer/Observer.h"

#include <QCalendarWidget>
#include <QPainter>
#include <QTextCharFormat>
#include <optional>

struct CalendarSelectionDTO
{
    std::optional<QDate> beginDate;
    std::optional<QDate> endDate;
};

class CalendarWidget
    : public QCalendarWidget
    , public ISubject<CalendarSelectionDTO>
{
    Q_OBJECT
public:
    CalendarWidget();

protected:
    void paintCell(QPainter* painter, const QRect& rect, QDate date) const override;

private:
    std::optional<QDate> beginDate;
    std::optional<QDate> endDate;
    std::unique_ptr<QTextCharFormat> highlighter;

    void performInitialSetup();
    void highlightRange(QTextCharFormat format);
    std::optional<QDate> dateFromPosition(const QPoint& position);

private slots:
    void selectDateRange(QDate date);
    void provideContextMenu(const QPoint& pos);
};

#endif // CALENDARWIDGET_H