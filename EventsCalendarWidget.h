#ifndef EVENTSCALENDARWIDGET_H
#define EVENTSCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QPainter>
#include <QTextCharFormat>
#include <optional>

class EventsCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    EventsCalendarWidget();

protected:
    void paintCell(QPainter* painter, const QRect& rect, QDate date) const;

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

#endif // EVENTSCALENDARWIDGET_H
