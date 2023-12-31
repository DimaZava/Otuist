#include "AddEventDialog.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

AddEventDialog::AddEventDialog(
    const std::set<SharedCalendarItem>& calendars,
    const DateTime& beginDateTime,
    const DateTime& endDateTime,
    QWidget* parent)
    : QDialog(parent)
    , calendars(calendars)
    , lastBeginDateTime(beginDateTime)
    , lastEndDateTime(endDateTime)
    , formLayout(std::make_unique<QFormLayout>(this))
    , nameTextEdit(std::make_unique<QTextEdit>(this))
    , calendarsComboBox(std::make_unique<QComboBox>(this))
    , categoriesComboBox(std::make_unique<QComboBox>(this))
    , wholeDayCheckbox(std::make_unique<QCheckBox>(this))
    , beginDateTimePicker(std::make_unique<QDateTimeEdit>(this))
    , endDateTimePicker(std::make_unique<QDateTimeEdit>(this))
    , descriptionTextEdit(std::make_unique<QTextEdit>(this))
    , saveButton(std::make_unique<QPushButton>())
{
    setupInitialState();
    configureForAdding();
}

AddEventDialog::AddEventDialog(
    const SharedCalendarEvent& event,
    const std::set<SharedCalendarItem>& calendars,
    QWidget* parent)
    : QDialog(parent)
    , calendars(calendars)
    , lastBeginDateTime(event->getBeginDateTime())
    , lastEndDateTime(event->getEndDateTime().value_or(CommonUtils::Time::nextHourToday(event->getBeginDateTime())))
    , formLayout(std::make_unique<QFormLayout>(this))
    , nameTextEdit(std::make_unique<QTextEdit>(this))
    , calendarsComboBox(std::make_unique<QComboBox>(this))
    , categoriesComboBox(std::make_unique<QComboBox>(this))
    , wholeDayCheckbox(std::make_unique<QCheckBox>(this))
    , beginDateTimePicker(std::make_unique<QDateTimeEdit>(this))
    , endDateTimePicker(std::make_unique<QDateTimeEdit>(this))
    , descriptionTextEdit(std::make_unique<QTextEdit>(this))
    , saveButton(std::make_unique<QPushButton>())
{
    setupInitialState();
    configureForEditing(event);
}

AddEventDialog::~AddEventDialog()
{
    qDebug() << __PRETTY_FUNCTION__;
}

AddEventDialogDTO AddEventDialog::getReturnValue()
{
    qDebug() << nameTextEdit->toPlainText().toStdString();
    qDebug() << calendarsComboBox->currentText().toStdString();
    qDebug() << categoriesComboBox->currentText().toStdString();
    qDebug() << wholeDayCheckbox->checkState();
    qDebug() << beginDateTimePicker->dateTime();
    qDebug() << endDateTimePicker->dateTime();
    qDebug() << descriptionTextEdit->toPlainText().toStdString();

    return AddEventDialogDTO{
        nameTextEdit->toPlainText().toStdString(),
        calendarsComboBox->currentText().toStdString(),
        categoriesComboBox->currentText().toStdString(),
        CommonUtils::Time::stdChronoTimePointFromQDateTime(beginDateTimePicker->dateTime()),
        wholeDayCheckbox->checkState() == Qt::CheckState::Checked
            ? std::nullopt
            : std::optional<DateTime>(
                  CommonUtils::Time::stdChronoTimePointFromQDateTime(endDateTimePicker->dateTime())),
        descriptionTextEdit->toPlainText().toStdString()};
}

void AddEventDialog::setupInitialState()
{
    setMinimumSize(InterfaceUtils::minimumAddEventDialogSize);
    resize(InterfaceUtils::minimumAddEventDialogSize);
    setWindowTitle(tr("Add Event"));

    nameTextEdit->setMinimumHeight(28);
    beginDateTimePicker->setMinimumHeight(28);
    endDateTimePicker->setMinimumHeight(28);
    descriptionTextEdit->setMinimumHeight(56);

    nameTextEdit->setTabChangesFocus(true);
    descriptionTextEdit->setTabChangesFocus(true);

    setTabOrder(nameTextEdit.get(), calendarsComboBox.get());
    setTabOrder(calendarsComboBox.get(), categoriesComboBox.get());
    setTabOrder(categoriesComboBox.get(), wholeDayCheckbox.get());
    setTabOrder(wholeDayCheckbox.get(), beginDateTimePicker.get());
    setTabOrder(beginDateTimePicker.get(), endDateTimePicker.get());
    setTabOrder(endDateTimePicker.get(), descriptionTextEdit.get());
    setTabOrder(descriptionTextEdit.get(), saveButton.get());

    formLayout->setContentsMargins(InterfaceUtils::defaultMargins);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->addRow(tr("Event Name"), nameTextEdit.get());
    formLayout->addRow(tr("Calendar Name"), calendarsComboBox.get());
    formLayout->addRow(tr("Category Name"), categoriesComboBox.get());
    formLayout->addRow(tr("Whole Day Event"), wholeDayCheckbox.get());
    formLayout->addRow(tr("Begin Date"), beginDateTimePicker.get());
    formLayout->addRow(tr("End Date"), endDateTimePicker.get());
    formLayout->addRow(tr("Description"), descriptionTextEdit.get());
    saveButton->setText(tr("OK"));
    formLayout->addRow(saveButton.get());

    connect(
        calendarsComboBox.get(), &QComboBox::currentTextChanged, this, &AddEventDialog::calendarComboboxValueChanged);

    beginDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateTimeFormat);
    connect(beginDateTimePicker.get(), &QDateTimeEdit::dateTimeChanged, this, &AddEventDialog::beginDateTimeChanged);

    endDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateTimeFormat);
    connect(endDateTimePicker.get(), &QDateTimeEdit::dateTimeChanged, this, &AddEventDialog::endDateTimeChanged);

    connect(wholeDayCheckbox.get(), &QCheckBox::stateChanged, this, &AddEventDialog::wholeDayCheckboxStateChange);

    connect(saveButton.get(), &QPushButton::clicked, this, &AddEventDialog::saveButtonDidClick);

    setLayout(formLayout.get());
}

void AddEventDialog::configureForAdding()
{
    for (const auto& calendar : calendars)
        calendarsComboBox->addItem(QString{calendar->getName().c_str()});
    calendarsComboBox->setCurrentIndex(0);
    calendarComboboxValueChanged(calendarsComboBox->currentText());

    const auto duration = lastEndDateTime - lastBeginDateTime;
    const auto days = std::chrono::duration_cast<std::chrono::days>(duration).count();

    wholeDayCheckbox->setCheckState(days == 0 ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    wholeDayCheckboxStateChange(wholeDayCheckbox->checkState());
}

void AddEventDialog::configureForEditing(const SharedCalendarEvent& event)
{
    nameTextEdit->setText(QString{event->getName().c_str()});

    for (const auto& calendar : calendars)
        calendarsComboBox->addItem(QString{calendar->getName().c_str()});
    calendarsComboBox->setCurrentIndex(0);
    calendarComboboxValueChanged(calendarsComboBox->currentText());

    // No need to set dates explicitly, since they are stored in last dates properties in ctr
    // and will be updated after calling wholeDayCheckboxStateChange
    wholeDayCheckbox->setCheckState(
        event->getEndDateTime().has_value() ? Qt::CheckState::Unchecked : Qt::CheckState::Checked);
    wholeDayCheckboxStateChange(wholeDayCheckbox->checkState());

    descriptionTextEdit->setText(QString{event->getDescription()->c_str()});
}

void AddEventDialog::calendarComboboxValueChanged(const QString& calendarName)
{
    if (auto it = std::find_if(
            calendars.begin(),
            calendars.end(),
            [&calendarName](const auto& calendar) {
        return calendar->getName() == calendarName.toStdString();
            });
        it != calendars.end())
    {
        categoriesComboBox->clear();
        auto categories = (*it)->getCategories();
        for (const auto& category : categories)
            categoriesComboBox->addItem(QString{category->name.c_str()});
    }
}

void AddEventDialog::wholeDayCheckboxStateChange(int newState)
{
    Qt::CheckState state = static_cast<Qt::CheckState>(newState);
    switch (state)
    {
        case Qt::Unchecked:
            beginDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateTimeFormat);
            endDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateTimeFormat);
            endDateTimePicker->setEnabled(true);
            break;
        case Qt::PartiallyChecked:
            break;
        case Qt::Checked:
            beginDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateFormat);
            endDateTimePicker->setDisplayFormat(CommonUtils::Time::qDateFormat);
            endDateTimePicker->setEnabled(false);
            break;
    }
    // Need to fix time clear while format change issue
    beginDateTimePicker->setDateTime(CommonUtils::Time::qDateTimeFromStdChrono(lastBeginDateTime));
    endDateTimePicker->setDateTime(CommonUtils::Time::qDateTimeFromStdChrono(lastEndDateTime));
}

void AddEventDialog::beginDateTimeChanged(const QDateTime& dateTime)
{
    lastBeginDateTime = CommonUtils::Time::stdChronoTimePointFromQDateTime(dateTime);
}

void AddEventDialog::endDateTimeChanged(const QDateTime& dateTime)
{
    lastEndDateTime = CommonUtils::Time::stdChronoTimePointFromQDateTime(dateTime);
}

void AddEventDialog::saveButtonDidClick(bool checked)
{
    accept();
}
