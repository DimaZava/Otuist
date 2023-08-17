#include "AddEventDialog.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

AddEventDialog::AddEventDialog(
    std::optional<std::chrono::time_point<std::chrono::system_clock>> beginDateTime,
    std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime,
    QWidget* parent)
    : QDialog(parent)
    , formLayout(std::make_unique<QFormLayout>(this))
    , nameTextEdit(std::make_unique<QTextEdit>(this))
    , calendarNameTextEdit(std::make_unique<QTextEdit>(this))
    , categoryNameTextEdit(std::make_unique<QTextEdit>(this))
    , beginDateTimePicker(std::make_unique<QDateTimeEdit>(this))
    , endDateTimePicker(std::make_unique<QDateTimeEdit>(this))
    , descriptionTextEdit(std::make_unique<QTextEdit>(this))
    , saveButton(std::make_unique<QPushButton>())
{
    setupInitialState(beginDateTime, endDateTime);
}

AddEventDialog::~AddEventDialog()
{
    qDebug() << __PRETTY_FUNCTION__;
}

AddEventDialogDTO AddEventDialog::getReturnValue()
{
    qDebug() << nameTextEdit->toPlainText().toStdString();
    qDebug() << calendarNameTextEdit->toPlainText().toStdString();
    qDebug() << categoryNameTextEdit->toPlainText().toStdString();
    qDebug() << beginDateTimePicker->dateTime();
    qDebug() << endDateTimePicker->dateTime();
    qDebug() << descriptionTextEdit->toPlainText().toStdString();

    return AddEventDialogDTO{
        nameTextEdit->toPlainText().toStdString(),
        calendarNameTextEdit->toPlainText().toStdString(),
        categoryNameTextEdit->toPlainText().toStdString(),
        CommonUtils::Time::stdChronoTimePointFromQDateTime(beginDateTimePicker->dateTime()),
        CommonUtils::Time::stdChronoTimePointFromQDateTime(endDateTimePicker->dateTime()),
        descriptionTextEdit->toPlainText().toStdString()};
}

void AddEventDialog::setupInitialState(
    std::optional<std::chrono::time_point<std::chrono::system_clock>> beginDateTime,
    std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime)
{
    setMinimumSize(InterfaceUtils::minimumAddEventDialogSize);
    resize(InterfaceUtils::minimumAddEventDialogSize);
    setWindowTitle(tr("Add Event"));

    nameTextEdit->setMinimumHeight(28);
    calendarNameTextEdit->setMinimumHeight(28);
    categoryNameTextEdit->setMinimumHeight(28);
    beginDateTimePicker->setMinimumHeight(28);
    endDateTimePicker->setMinimumHeight(28);
    descriptionTextEdit->setMinimumHeight(56);

    nameTextEdit->setTabChangesFocus(true);
    calendarNameTextEdit->setTabChangesFocus(true);
    categoryNameTextEdit->setTabChangesFocus(true);
    descriptionTextEdit->setTabChangesFocus(true);

    setTabOrder(nameTextEdit.get(), calendarNameTextEdit.get());
    setTabOrder(calendarNameTextEdit.get(), categoryNameTextEdit.get());
    setTabOrder(categoryNameTextEdit.get(), beginDateTimePicker.get());
    setTabOrder(beginDateTimePicker.get(), endDateTimePicker.get());
    setTabOrder(endDateTimePicker.get(), descriptionTextEdit.get());
    setTabOrder(descriptionTextEdit.get(), saveButton.get());

    formLayout->setContentsMargins(InterfaceUtils::defaultMargins);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->addRow(tr("Event Name"), nameTextEdit.get());
    formLayout->addRow(tr("Calendar Name"), calendarNameTextEdit.get());
    formLayout->addRow(tr("Category Name"), categoryNameTextEdit.get());
    formLayout->addRow(tr("Begin Date"), beginDateTimePicker.get());
    formLayout->addRow(tr("End Date"), endDateTimePicker.get());
    formLayout->addRow(tr("Description"), descriptionTextEdit.get());
    saveButton->setText(tr("OK"));
    formLayout->addRow(saveButton.get());

    beginDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateTimeFormat);
    if (beginDateTime.has_value())
    {
        beginDateTimePicker->setDateTime(CommonUtils::Time::qDateTimeFromStdChrono(beginDateTime.value()));
    }

    endDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateTimeFormat);
    if (endDateTime.has_value())
    {
        endDateTimePicker->setDateTime(CommonUtils::Time::qDateTimeFromStdChrono(endDateTime.value()));
    }
    else if (beginDateTime.has_value())
    {
        endDateTimePicker->setDateTime(
            CommonUtils::Time::qDateTimeFromStdChrono(CommonUtils::Time::endOfDate(beginDateTime.value())));
    }

    connect(saveButton.get(), &QPushButton::clicked, this, &AddEventDialog::saveButtonDidClick);

    setLayout(formLayout.get());
}

void AddEventDialog::saveButtonDidClick(bool checked)
{
    accept();
}
