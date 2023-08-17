#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include "../../BusinessLogicLayer/CalendarsRepository/CalendarsRepository.h"
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
    , public IObserver<std::set<std::shared_ptr<CalendarEvent>>>
{
    Q_OBJECT
public:
    explicit CalendarWidget(const std::shared_ptr<CalendarsRepository>& calendarsRepository);
    ~CalendarWidget() override;
    void didChange(const std::set<std::shared_ptr<CalendarEvent>>& value) override;

protected:
    void paintCell(QPainter* painter, const QRect& rect, QDate date) const override;

private:
    std::optional<QDate> beginDate;
    std::optional<QDate> endDate;
    std::unique_ptr<QTextCharFormat> highlighter;
    int currentMonth;
    const std::shared_ptr<CalendarsRepository>& calendarsRepository;

    void performInitialSetup();
    void highlightRange(QTextCharFormat format);
    std::optional<QDate> dateFromPosition(const QPoint& position);

private slots:
    void selectDateRange(QDate date);
    void provideContextMenu(const QPoint& pos);
    void currentPageDidChange(int year, int month);
};

#endif // CALENDARWIDGET_H
