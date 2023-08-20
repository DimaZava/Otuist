#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../../ModelsLayer/CalendarItem/CalendarItem.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QTextEdit>

struct AddEventDialogDTO
{
    std::string name;
    std::string calendarName;
    std::string category;
    DateTime beginDateTime;
    std::optional<DateTime> endDateTime;
    std::optional<std::string> description;
};

class AddEventDialog : public QDialog
{
    Q_OBJECT
public:
    AddEventDialog(
        const std::set<SharedCalendarItem>& calendars,
        const DateTime& beginDateTime,
        const DateTime& endDateTime,
        QWidget* parent = nullptr);

    AddEventDialog(
        const SharedCalendarEvent& event,
        const std::set<SharedCalendarItem>& calendars,
        QWidget* parent = nullptr);

    AddEventDialog(const AddEventDialog& dialog) = delete;
    AddEventDialog(AddEventDialog&& dialog) = delete;

    ~AddEventDialog() override;

    AddEventDialog& operator=(const AddEventDialog& dialog) = delete;
    AddEventDialog& operator=(AddEventDialog&& dialog) = delete;

    AddEventDialogDTO getReturnValue();

private:
    void setupInitialState();
    void configureForAdding();
    void configureForEditing(const SharedCalendarEvent& event);

    std::set<SharedCalendarItem> calendars;
    std::unique_ptr<QFormLayout> formLayout;
    std::unique_ptr<QTextEdit> nameTextEdit;
    std::unique_ptr<QComboBox> calendarsComboBox;
    std::unique_ptr<QComboBox> categoriesComboBox;
    std::unique_ptr<QCheckBox> wholeDayCheckbox;
    std::unique_ptr<QDateTimeEdit> beginDateTimePicker;
    std::unique_ptr<QDateTimeEdit> endDateTimePicker;
    std::unique_ptr<QTextEdit> descriptionTextEdit;
    std::unique_ptr<QPushButton> saveButton;

    DateTime lastBeginDateTime;
    DateTime lastEndDateTime;

private slots:
    void calendarComboboxValueChanged(const QString& calendarName);
    void wholeDayCheckboxStateChange(int newState);
    void beginDateTimeChanged(const QDateTime& dateTime);
    void endDateTimeChanged(const QDateTime& dateTime);
    void saveButtonDidClick(bool checked);
};

#endif // ADDEVENTDIALOG_H
