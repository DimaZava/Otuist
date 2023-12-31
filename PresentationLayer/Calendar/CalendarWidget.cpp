#include "CalendarWidget.h"
#include "../AddEventDialog/AddEventDialog.h"
#include "../InterfaceUtils.h"

#include <QApplication>
#include <QMenu>
#include <QPalette>
#include <QTableView>

CalendarWidget::CalendarWidget(const std::shared_ptr<CalendarsManager>& calendarsManager)
    : QCalendarWidget()
    , calendarsManager(calendarsManager)
    , highlighter(std::make_unique<QTextCharFormat>())
    , currentMonth(QDate::currentDate().month())
{
    calendarsManager->addObserver(this);
    performInitialSetup();
}

CalendarWidget::~CalendarWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
    calendarsManager->removeObserver(this);
}

void CalendarWidget::didChange(const std::set<SharedCalendarEvent>& value)
{
    updateCells();
}

void CalendarWidget::performInitialSetup()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QCalendarWidget::customContextMenuRequested, this, &CalendarWidget::provideContextMenu);
    connect(this, &QCalendarWidget::currentPageChanged, this, &CalendarWidget::currentPageDidChange);

    highlighter->setBackground(palette().brush(QPalette::Highlight));
    highlighter->setForeground(palette().brush(QPalette::HighlightedText));

    connect(this, &QCalendarWidget::clicked, this, &CalendarWidget::selectDateRange);
    dateTextFormat();

    // Force to highlight and show events data
    selectDateRange(selectedDate());
}

void CalendarWidget::selectDateRange(QDate date)
{
    highlightRange(QTextCharFormat{});

    if (qApp->keyboardModifiers() & Qt::ShiftModifier && beginDate.has_value())
    {
        this->endDate = date;
        if (date < this->beginDate.value())
            std::swap(this->beginDate, this->endDate);
        highlightRange(*highlighter);
    }
    else
    {
        this->beginDate = date;
        this->endDate = date;
    }

    if (!this->beginDate.has_value() && !this->endDate.has_value())
        static_assert(true, "Both dates should have values");
    notify({this->beginDate.value(), this->endDate.value()});
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
    const QTableView* const view = findChild<const QTableView*>();
    const QPoint viewportPos = view->viewport()->mapFromGlobal(item);
    const QModelIndex clickedIndex = view->indexAt(viewportPos);

    if (!clickedIndex.isValid() || clickedIndex.column() == 0 || clickedIndex.row() == 0)
        return;

    QMenu submenu;

    submenu.addAction(tr("Add"));
    submenu.addAction(tr("Delete All"));
    QAction* rightClickItem = submenu.exec(item);

    if (rightClickItem == nullptr)
        return;

    if (rightClickItem->text() == tr("Add"))
    {
        // Multidate case: get some predefined dates based on selections
        if (beginDate.has_value() && endDate.has_value() && (beginDate.value() != endDate.value()))
        {
            auto convertedBeginRangeDate = CommonUtils::Time::stdChronoTimePointFromQDate(beginDate.value());
            auto convertedEndRangeDate =
                CommonUtils::Time::endOfDate(CommonUtils::Time::stdChronoTimePointFromQDate(endDate.value()));

            AddEventDialog dialog(
                calendarsManager->getCalendars(), convertedBeginRangeDate, convertedEndRangeDate, this);
            if (dialog.exec() == QDialog::Accepted)
            {
                auto addEventDTO = dialog.getReturnValue();
                calendarsManager->addEvent(std::make_shared<CalendarEvent>(
                    addEventDTO.name,
                    addEventDTO.calendarName,
                    addEventDTO.category,
                    addEventDTO.beginDateTime,
                    addEventDTO.endDateTime,
                    addEventDTO.description));
            }
        }
        // Single date case: get date from coursor position
        else if (const auto addDate = dateFromPosition(item); addDate.has_value())
        {
            // Position relative hour selection
            const QRect rect = view->visualRect(clickedIndex);
            const QPoint localPoint = viewportPos - rect.topLeft();
            const double relativeVerticalPosition = double(localPoint.y()) / double(rect.height());

            const int adjustedBeginHours = std::min(23, int(round(24 * relativeVerticalPosition))); // 24h is illegal
            const int adjustedEndHours = adjustedBeginHours != 23 ? adjustedBeginHours + 1 : adjustedBeginHours;
            const int adjustedEndMinutes = adjustedBeginHours != 23 ? 0 : 59;

            const auto beginTime = QTime(adjustedBeginHours, 0);
            const auto endTime = QTime(adjustedEndHours, adjustedEndMinutes);
            const auto beginDateTime = QDateTime(addDate.value(), beginTime);
            const auto endDateTime = QDateTime(addDate.value(), endTime);
            const auto convertedBeginDateTime = CommonUtils::Time::stdChronoTimePointFromQDateTime(beginDateTime);
            const auto convertedEndDateTime = CommonUtils::Time::stdChronoTimePointFromQDateTime(endDateTime);
            qDebug() << "Add " << CommonUtils::Time::stringFromStdChrono(convertedBeginDateTime) << "-"
                     << CommonUtils::Time::stringFromStdChrono(convertedEndDateTime);

            AddEventDialog dialog(calendarsManager->getCalendars(), convertedBeginDateTime, convertedEndDateTime, this);
            if (dialog.exec() == QDialog::Accepted)
            {
                auto addEventDTO = dialog.getReturnValue();
                calendarsManager->addEvent(std::make_shared<CalendarEvent>(
                    addEventDTO.name,
                    addEventDTO.calendarName,
                    addEventDTO.category,
                    addEventDTO.beginDateTime,
                    addEventDTO.endDateTime,
                    addEventDTO.description));
            }
        }
    }
    else if (rightClickItem->text() == tr("Delete All"))
    {
        auto deleteDate = dateFromPosition(item);
        if (!deleteDate.has_value())
            return;

        auto eventsToRemove = calendarsManager->getEvents(
            CommonUtils::Time::stdChronoTimePointFromQDate(deleteDate.value()), std::nullopt, false);

        const int ret = InterfaceUtils::showConfirmationAlert(
            tr("Delete Events"),
            tr("Do you really want to delete %1 event(s)?").arg(eventsToRemove.size()),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);

        switch (ret)
        {
            case QMessageBox::Ok:
                calendarsManager->removeEvents(eventsToRemove);
                break;
            case QMessageBox::Cancel:
                break;
            default:
                static_assert(true, "Unexpected condition");
        }
    }
}

void CalendarWidget::currentPageDidChange(int year, int month)
{
    currentMonth = month;
}

const QPen CalendarWidget::bordersPen()
{
    QPen bordersPen = QPen(QColor{64, 64, 64});
    bordersPen.setWidth(1);
    return bordersPen;
}

const QPen CalendarWidget::datePen(QPen bordersPen)
{
    QPen datePen = QPen(Qt::white);
    bordersPen.setWidth(1);
    return datePen;
}

const QPen CalendarWidget::nonCurrentMonthDatePen()
{
    QPen nonCurrentMonthDatePen = QPen(QColor{64, 64, 64});
    nonCurrentMonthDatePen.setWidth(1);
    return nonCurrentMonthDatePen;
}

const QPen CalendarWidget::eventsCounterPen()
{
    QPen eventsCounterPen = QPen(Qt::lightGray);
    eventsCounterPen.setWidth(1);
    return eventsCounterPen;
}

const QBrush CalendarWidget::todayIndicatorBrush()
{
    QBrush todayIndicatorBrush = QBrush(Qt::red);
    return todayIndicatorBrush;
}

void CalendarWidget::paintCell(QPainter* painter, const QRect& rect, QDate date) const
{
    QCalendarWidget::paintCell(painter, rect, date);

    const auto beginDate = CommonUtils::Time::stdChronoTimePointFromQDate(date);
    const auto events = calendarsManager->getEvents(beginDate, std::nullopt, false);
    const size_t eventsSize = events.size();

    const QPen bordersPen = CalendarWidget::bordersPen();
    const QPen datePen = CalendarWidget::datePen(bordersPen);
    const QPen nonCurrentMonthDatePen = CalendarWidget::nonCurrentMonthDatePen();

    const QPen eventsCounterPen = CalendarWidget::eventsCounterPen();
    QRect eventsCounterRect{rect};
    eventsCounterRect.adjust(0, 0, -eventsCounterRect.width() + 20, -eventsCounterRect.height() + 20);

    const int centerTextDimension = 20;
    const QBrush todayIndicatorBrush = CalendarWidget::todayIndicatorBrush();
    QRect dateRect{rect};
    dateRect.adjust(
        dateRect.width() / 2 - centerTextDimension / 2 + 1, // Magical adjustment, highly likely caused by pen width
        dateRect.height() / 2 - centerTextDimension / 2 + 1,
        -dateRect.width() / 2 + centerTextDimension / 2 + 1,
        -dateRect.height() / 2 + centerTextDimension / 2 + 1);

    painter->save();
    {
        painter->setPen(bordersPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);

        if (eventsSize > 0)
        {
            painter->setPen(eventsCounterPen);
            painter->drawRect(eventsCounterRect);
            painter->drawText(
                eventsCounterRect, Qt::TextSingleLine | Qt::AlignCenter, "+" + QString::number(events.size()));
        }
    }
    painter->restore();

    painter->save();
    {
        if (date == QDate::currentDate())
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(todayIndicatorBrush);
            painter->drawEllipse(dateRect.center(), centerTextDimension / 2, centerTextDimension / 2);
        }

        if (date.month() == currentMonth)
            painter->setPen(datePen);
        else
            painter->setPen(nonCurrentMonthDatePen);
        painter->setBrush(Qt::NoBrush);

        painter->drawText(rect, Qt::TextSingleLine | Qt::AlignCenter, QString::number(date.day()));
    }
    painter->restore();
}
