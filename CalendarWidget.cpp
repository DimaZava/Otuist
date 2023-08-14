#include "CalendarWidget.h"

#include <QApplication>
#include <QMenu>
#include <QPalette>
#include <QTableView>

CalendarWidget::CalendarWidget()
    : QCalendarWidget()
    , highlighter(std::make_unique<QTextCharFormat>())
{
    performInitialSetup();
}

void CalendarWidget::performInitialSetup()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QCalendarWidget::customContextMenuRequested, this, &CalendarWidget::provideContextMenu);

    highlighter->setBackground(palette().brush(QPalette::Highlight));
    highlighter->setForeground(palette().brush(QPalette::HighlightedText));

    connect(this, &QCalendarWidget::clicked, this, &CalendarWidget::selectDateRange);
    dateTextFormat();
}

void CalendarWidget::selectDateRange(QDate date)
{
    highlightRange(QTextCharFormat{});

    if (qApp->keyboardModifiers() & Qt::ShiftModifier && beginDate.has_value())
    {
        this->endDate = date;
        highlightRange(*highlighter);
    }
    else
    {
        this->beginDate = date;
        this->endDate = std::nullopt;
    }
}

void CalendarWidget::highlightRange(QTextCharFormat format)
{
    if (!beginDate.has_value() || !endDate.has_value())
        return;

    QDate beg = beginDate.value() < endDate.value() ? beginDate.value() : endDate.value();
    QDate end = beginDate.value() > endDate.value() ? beginDate.value() : endDate.value();

    while (beg <= end)
    {
        setDateTextFormat(beg, format);
        beg = beg.addDays(1);
    }
}

std::optional<QDate> CalendarWidget::dateFromPosition(const QPoint& position)
{
    const QTableView* const view = findChild<const QTableView*>();
    const QAbstractItemModel* const model = view->model();
    const int startCol = verticalHeaderFormat() == QCalendarWidget::NoVerticalHeader ? 0 : 1;
    const int startRow = horizontalHeaderFormat() == QCalendarWidget::NoHorizontalHeader ? 0 : 1;
    const QModelIndex clickedIndex = view->indexAt(view->viewport()->mapFromGlobal(position));

    if (clickedIndex.row() < startRow || clickedIndex.column() < startCol)
        return std::nullopt;

    QModelIndex firstIndex;
    bool firstFound = false;
    for (int i = startRow, maxI = model->rowCount(); !firstFound && i < maxI; ++i)
    {
        for (int j = startCol, maxJ = model->columnCount(); !firstFound && j < maxJ; ++j)
        {
            firstIndex = model->index(i, j);
            if (firstIndex.data().toInt() == 1)
                firstFound = true;
        }
    }
    const int lastDayMonth = QDate(yearShown(), monthShown(), 1).addMonths(1).addDays(-1).day();
    bool lastFound = false;
    QModelIndex lastIndex;
    for (int i = model->rowCount() - 1, minI = firstIndex.row(); !lastFound && i >= minI; --i)
    {
        for (int j = model->columnCount() - 1; !lastFound && j >= startCol; --j)
        {
            lastIndex = model->index(i, j);
            if (lastIndex.data().toInt() == lastDayMonth)
                lastFound = true;
        }
    }
    int monthShift = 0;
    int yearShift = 0;
    if (clickedIndex.row() < firstIndex.row() ||
        (clickedIndex.row() == firstIndex.row() && clickedIndex.column() < firstIndex.column()))
    {
        if (monthShown() == 1)
        {
            yearShift = -1;
            monthShift = 11;
        }
        else
            monthShift = -1;
    }
    else if (
        clickedIndex.row() > lastIndex.row() ||
        (clickedIndex.row() == lastIndex.row() && clickedIndex.column() > lastIndex.column()))
    {
        if (monthShown() == 12)
        {
            yearShift = 1;
            monthShift = -11;
        }
        else
            monthShift = 1;
    }

    const QDate resultDate = QDate(yearShown() + yearShift, monthShown() + monthShift, clickedIndex.data().toInt());
    qDebug() << resultDate;
    return resultDate;
}

void CalendarWidget::provideContextMenu(const QPoint& pos)
{
    QPoint item = this->mapToGlobal(pos);
    QMenu submenu;

    submenu.addAction("Add");
    submenu.addAction("Delete");
    QAction* rightClickItem = submenu.exec(item);

    if (rightClickItem->text() == "Add")
    {
        auto addDate = dateFromPosition(item);
        qDebug() << "Add " << addDate.value().toString();
    }
    else if (rightClickItem->text() == "Delete")
    {
        auto deleteDate = dateFromPosition(item);
        qDebug() << "Delete " << deleteDate.value().toString();
    }
}

void CalendarWidget::paintCell(QPainter* painter, const QRect& rect, QDate date) const
{
    QCalendarWidget::paintCell(painter, rect, date);
    // draw calendar cell marks
    // add list?

    painter->save();
    painter->drawRect(rect.x(), rect.y(), 15, 15);
    painter->fillRect(rect.x(), rect.y(), 15, 15, Qt::red);
    painter->drawText(rect, Qt::TextSingleLine | Qt::AlignCenter, QString::number(date.day()));
    painter->restore();
}
