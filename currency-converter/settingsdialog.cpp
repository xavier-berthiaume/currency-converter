#include <QDir>
#include <QDebug>
#include <QComboBox>
#include <QPushButton>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(const QString &currentLang, QWidget *parent)
    : currentLang(currentLang)
    , QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    populateLanguages();

    QDialogButtonBox *buttons = findChild<QDialogButtonBox *>("buttonBox");
    QPushButton *applyButton = buttons->button(QDialogButtonBox::Apply); // For some reason the apply button doesn't emit the accepted signal, so manually connecting

    qDebug() << "Connecting close button functionality: " << connect(buttons, &QDialogButtonBox::rejected, this, &SettingsDialog::onCloseButtonClicked);
    qDebug() << "Connecting save button functionality: " << connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::onSaveButtonClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::populateLanguages()
{
    QComboBox *lcombo = findChild<QComboBox *>("languageComboBox");
    if (!lcombo) {
        qDebug() << "Could not find languageComboBox!";
        return;
    }

    QDir translationsDir("translations"); // Directory containing .qm files
    if (!translationsDir.exists()) {
        qDebug() << "Translations directory does not exist!";
        return;
    }

    QStringList filters = {"*.qm"};
    QStringList translationFiles = translationsDir.entryList(filters, QDir::Files);

    for (const QString &filename : translationFiles) {
        // Extract the language and locale code from the filename
        QString baseName = filename.split('.').first(); // Remove the .qm or .ts extension
        QStringList parts = baseName.split('_');       // Split by underscore

        // Check for valid language and locale
        QString languageCode = parts.size() > 1 ? parts[1] : ""; // First part after "currency-converter"
        QString localeCode = parts.size() > 2 ? parts[2] : "";   // Second part (e.g., CA for Canadian French)
        QString languageName;

        if (languageCode == "en") {
            if (localeCode == "US") {
                languageName = "English (US)";
            } else if (localeCode == "GB") {
                languageName = "English (UK)";
            } else {
                languageName = "English";
            }
        } else if (languageCode == "fr") {
            if (localeCode == "CA") {
                languageName = "Français (Canada)";
            } else if (localeCode == "FR") {
                languageName = "Français (France)";
            } else {
                languageName = "Français";
            }
        } else {
            languageName = QString("%1 (%2)").arg(languageCode.toUpper(), localeCode.toUpper());
        }

        qDebug() << "Adding to lanugage options " << languageName << " With data code " << languageCode;
        lcombo->addItem(languageName, languageCode + "_" + localeCode);
        if (languageCode + "_" + localeCode == currentLang) {
            int index = lcombo->count() - 1;
            lcombo->setCurrentIndex(index);

            qDebug() << "Set the current index of the language combobox to " << index;
        }
    }
}

QString SettingsDialog::getSelectedLanguage()
{
    QComboBox *lcombo = findChild<QComboBox *>("languageComboBox");

    return lcombo->currentData().toString();
}

void SettingsDialog::onCloseButtonClicked()
{
    qDebug() << "Close button clicked";
    this->reject();
}

void SettingsDialog::onSaveButtonClicked()
{
    qDebug() << "Apply button clicked";
    this->accept();
}
