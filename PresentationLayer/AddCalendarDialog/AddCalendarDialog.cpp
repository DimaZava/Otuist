#include "AddCalendarDialog.h"
#include "../InterfaceUtils.h"

AddCalendarDialog::AddCalendarDialog(QWidget* parent)
    : QDialog(parent)
    , formLayout(std::make_unique<QFormLayout>(this))
    , nameTextEdit(std::make_unique<QTextEdit>(this))
    , categoriesContainerWidget(std::make_unique<QWidget>(this))
    , categoriesLayout(std::make_unique<QVBoxLayout>(categoriesContainerWidget.get()))
    , categoriesListScrollArea(std::make_unique<QScrollArea>())
    , categoriesListContainerWidget(std::make_unique<QWidget>(this))
    , categoriesList(std::make_unique<QVBoxLayout>())
    , categoriesToolbar(std::make_unique<QToolBar>())
    , saveButton(std::make_unique<QPushButton>())
    , addAction(std::make_unique<QAction>("+", this))
    , removeAction(std::make_unique<QAction>("-", this))
{
    setupInitialState();
}

AddCalendarDialog::~AddCalendarDialog()
{
    qDebug() << __PRETTY_FUNCTION__;
}

AddCalendarDTO AddCalendarDialog::getReturnValue()
{
    std::set<std::string> categories;
    auto calendarName = nameTextEdit->toPlainText().toStdString();
    for (int i = 0; i < categoriesList->count(); ++i)
    {
        auto widget = categoriesList->itemAt(i)->widget();
        if (widget == nullptr)
            break;
        auto textEdit = dynamic_cast<QTextEdit*>(widget);
        if (textEdit == nullptr)
            break;
        categories.insert(textEdit->toPlainText().toStdString());
    }
    return AddCalendarDTO{calendarName, categories};
}

void AddCalendarDialog::setupInitialState()
{
    setWindowTitle(tr("Add Calendar"));
    nameTextEdit->setFixedHeight(28);

    categoriesContainerWidget->setMinimumHeight(128);
    categoriesContainerWidget->setLayout(categoriesLayout.get());
    categoriesLayout->setContentsMargins(InterfaceUtils::zeroMargins);
    categoriesLayout->setSpacing(0);

    categoriesLayout->addWidget(categoriesListScrollArea.get());
    categoriesListScrollArea->setWidgetResizable(true);

    categoriesListScrollArea->setWidget(categoriesListContainerWidget.get());
    categoriesListContainerWidget->setLayout(categoriesList.get());
    categoriesList->setContentsMargins(InterfaceUtils::zeroMargins);
    categoriesList->setSpacing(0);
    categoriesList->addStretch();

    connect(addAction.get(), &QAction::triggered, this, &AddCalendarDialog::addCategoryActionTriggered);
    connect(removeAction.get(), &QAction::triggered, this, &AddCalendarDialog::removeCategoryActionTriggered);
    categoriesToolbar->addActions({addAction.get(), removeAction.get()});
    categoriesLayout->addWidget(categoriesToolbar.get());

    saveButton->setText(tr("OK"));
    connect(saveButton.get(), &QPushButton::clicked, this, &AddCalendarDialog::saveButtonDidClick);

    formLayout->setContentsMargins(InterfaceUtils::defaultMargins);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->addRow(tr("Calendar Name"), nameTextEdit.get());
    formLayout->addRow(tr("Add Categories"), categoriesContainerWidget.get());
    formLayout->addRow(saveButton.get());
}

void AddCalendarDialog::saveButtonDidClick(bool checked)
{
    accept();
}

void AddCalendarDialog::addCategoryActionTriggered()
{
    QTextEdit* categoryTextEdit = new QTextEdit;
    categoryTextEdit->setFixedHeight(28);
    // Insert at pre-last item to keep bottom stretch correctly
    categoriesList->insertWidget(categoriesList->count() - 1, categoryTextEdit);
}

void AddCalendarDialog::removeCategoryActionTriggered()
{
    auto selectedWidget = focusWidget();
    if (selectedWidget->parentWidget()->layout() != categoriesList.get())
        return;
    delete selectedWidget;
}
