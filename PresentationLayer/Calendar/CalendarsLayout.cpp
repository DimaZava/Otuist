#include "CalendarsLayout.h"
#include "../../PresentationLayer/InterfaceUtils.h"
#include "../AddCalendarDialog/AddCalendarDialog.h"

#include <QHeaderView>
#include <set>

enum class WidgetCalendarColumns
{
    categoryName = 0
};

enum class WidgetCategoryColumns
{
    checkState = 0,
    categoryName
};

CalendarsLayout::CalendarsLayout(const std::shared_ptr<CalendarsRepository>& calendarsRepository, QWidget* parent)
    : QVBoxLayout(parent)
    , calendarsRepository(calendarsRepository)
    , categoriesTree(std::make_unique<QTreeWidget>(new QTreeWidget))
    , toolbar(std::make_unique<QToolBar>())
    , addAction(std::make_unique<QAction>("+", this))
    , removeAction(std::make_unique<QAction>("-", this))
{
    configureLayout();
}

CalendarsLayout::~CalendarsLayout()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void CalendarsLayout::configureLayout()
{
    setSpacing(0);

    categoriesTree->setHeaderHidden(true);
    categoriesTree->setHeaderLabels({"Is Enabled", "Title"});
    categoriesTree->header()->setSectionResizeMode(QHeaderView::Stretch);

    categoriesTree->setContentsMargins(InterfaceUtils::zeroMargins);
    categoriesTree->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    categoriesTree->setColumnWidth(0, 25);
    // categoriesTree->setColumnWidth(1, 100);

    connect(categoriesTree.get(), &QTreeWidget::itemChanged, this, &CalendarsLayout::treeItemDidChange);
    connect(categoriesTree.get(), &QTreeWidget::itemClicked, this, &CalendarsLayout::treeActiveItemClicked);
    addWidget(categoriesTree.get());

    connect(addAction.get(), &QAction::triggered, this, &CalendarsLayout::addCalendarActionTriggered);
    connect(removeAction.get(), &QAction::triggered, this, &CalendarsLayout::removeCalendarActionTriggered);

    toolbar->setContentsMargins(InterfaceUtils::zeroMargins);
    toolbar->addActions({addAction.get(), removeAction.get()});
    addWidget(toolbar.get());

    reloadData();
}

void CalendarsLayout::reloadData()
{
    categoriesTree->clear();

    for (const auto& calendarItem : calendarsRepository->getCalendars())
    {
        QTreeWidgetItem* topWidgetItem = new QTreeWidgetItem{categoriesTree.get()};
        topWidgetItem->setText(0, calendarItem.get()->getName().c_str());

        for (const auto& category : calendarItem.get()->getCategories())
        {
            QTreeWidgetItem* categoryWidgetItem = new QTreeWidgetItem{topWidgetItem};
            categoryWidgetItem->setCheckState(
                static_cast<int>(WidgetCategoryColumns::checkState), Qt::CheckState::Checked);
            categoryWidgetItem->setText(static_cast<int>(WidgetCategoryColumns::categoryName), category->name.c_str());
        }
    }

    if (categoriesTree->topLevelItem(0) != nullptr)
        categoriesTree->setCurrentItem(categoriesTree->topLevelItem(0));

    categoriesTree->expandAll();
}

void CalendarsLayout::treeItemDidChange(QTreeWidgetItem* item, int column)
{
    QTreeWidgetItem* calendarTreeWidgetItem = item->parent();
    if (calendarTreeWidgetItem == nullptr)
        return;

    std::string calendarName =
        calendarTreeWidgetItem->text(static_cast<int>(WidgetCalendarColumns::categoryName)).toStdString();
    bool newCheckState = item->checkState(static_cast<int>(WidgetCategoryColumns::checkState));
    std::string categoryName = item->text(static_cast<int>(WidgetCategoryColumns::categoryName)).toStdString();
    calendarsRepository->setCalendarsCategoryActive(calendarName, categoryName, newCheckState);
}

void CalendarsLayout::addCalendarActionTriggered()
{
    AddCalendarDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        auto addCalendarDTO = dialog.getReturnValue();
        auto dtoCategories = addCalendarDTO.categories;
        std::set<std::shared_ptr<CalendarCategory>, CalendarCategoryComparator> categories;

        std::transform(
            addCalendarDTO.categories.begin(),
            addCalendarDTO.categories.end(),
            std::inserter(categories, categories.end()),
            [&dtoCategories](const std::string stringCategory) {
            return std::make_shared<CalendarCategory>(stringCategory);
            });

        calendarsRepository->addCalendar(std::make_shared<CalendarItem>(addCalendarDTO.name, categories));
        reloadData();
    }
}

void CalendarsLayout::removeCalendarActionTriggered()
{
    std::string calendarName =
        categoriesTree->currentItem()->text(static_cast<int>(WidgetCalendarColumns::categoryName)).toStdString();

    auto calendar = calendarsRepository->getCalendar(calendarName);
    if (!calendar.has_value())
        return;

    const int ret = InterfaceUtils::showConfirmationAlert(
        tr("Delete Events"),
        tr("You really want to delete calendar %1 with all %2 events?")
            .arg(QString(calendarName.c_str()), QString::number(calendar.value()->getEvents().size())),
        QMessageBox::Ok | QMessageBox::Cancel,
        QMessageBox::Ok);

    switch (ret)
    {
        case QMessageBox::Ok:
            calendarsRepository->deleteCalendar(calendarName);
            reloadData();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            static_assert(true, "Unexpected condition");
    }
}

void CalendarsLayout::treeActiveItemClicked(QTreeWidgetItem* item, int column)
{
    if (item->parent() != nullptr)
    {
        removeAction->setEnabled(false);
        return;
    }
    removeAction->setEnabled(true);
}
