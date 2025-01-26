#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QMessageBox>

#include "settingsdialog.h"
#include "creditdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentLang("fr_CA")
    , m_langPath(QDir(QCoreApplication::applicationDirPath()).filePath("translations"))
{
    ui->setupUi(this);

    setWindowTitle(tr("Currency-Converter"));

    if (!m_translator.load(QString("currency-converter_%1").arg(m_currentLang), m_langPath)) {
        qDebug() << "Failed to load the translation file:"
                 << m_langPath + "/currency-converter_" + m_currentLang + ".qm";
    } else {
        qApp->installTranslator(&m_translator);
        ui->retranslateUi(this);
        qDebug() << "Loaded translation file:"
                 << m_langPath + "/currency-converter_" + m_currentLang + ".qm";
    }

    QDoubleSpinBox *currency1spin = findChild<QDoubleSpinBox *>("currency1SpinBox");
    QDoubleSpinBox *currency2spin = findChild<QDoubleSpinBox *>("currency2SpinBox");

    currency1spin->setDecimals(2);
    currency2spin->setDecimals(2);

    currency1spin->setRange(0, 9e9);
    currency2spin->setRange(0, 9e9);

    currency1spin->setSingleStep(1.00);
    currency2spin->setSingleStep(1.00);


    apiManager = new CurrencyAPIManager(this);

    // Connect network API signals
    connect(apiManager, &CurrencyAPIManager::currenciesFetched, this, &MainWindow::updateCurrencies);
    connect(apiManager, &CurrencyAPIManager::exchangeRatesFetched, this, &MainWindow::updateExchange);
    connect(apiManager, &CurrencyAPIManager::errorFetchingCurrencies, this, &MainWindow::networkError);

    apiManager->fetchCurrencies();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateJumbotron(const QString &currency1, const double &value1, const QString &currency2, const double &value2)
{
    QLabel *left = findChild<QLabel *>("currency1Label");
    QLabel *right = findChild<QLabel *>("currency2Label");

    left->setText(QString::number(value1, 'f', 2) + " " + currency1.toUpper());
    right->setText(QString::number(value2, 'f', 2) + " " + currency2.toUpper());
}

void MainWindow::updateCurrency2Spinbox(const double &value)
{
    QDoubleSpinBox *currency2spin = findChild<QDoubleSpinBox *>("currency2SpinBox");

    currency2spin->setValue(value);
}

void MainWindow::changeLanguage(const QString &language)
{
    if (language == m_currentLang) {
        return; // Already loaded
    }

    // Remove the current translator
    qApp->removeTranslator(&m_translator);

    // Load the new translation file
    if (m_translator.load(QString("currency-converter_%1").arg(language), m_langPath)) {
        qApp->installTranslator(&m_translator);
        m_currentLang = language;

        // Reapply translations
        ui->retranslateUi(this);

        qDebug() << "Language changed to:" << language;
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load language: %1").arg(language));
        qDebug() << "Failed to load translation file for" << language;
    }
}

void MainWindow::updateCurrencies(const QMap<QString, QString> &currencies)
{
    qDebug() << "Removing loading labels";

    QLabel *currency1Loading = findChild<QLabel *>("currency1Loading");
    QLabel *currency2Loading = findChild<QLabel *>("currency2Loading");

    if (currency1Loading) {
        currency1Loading->hide();
    }

    if (currency2Loading) {
        currency2Loading->hide();
    }

    QComboBox *currency1combo = findChild<QComboBox *>("currency1ComboBox");
    QComboBox *currency2combo = findChild<QComboBox *>("currency2ComboBox");

    currency1combo->blockSignals(true); // Block signals temporarily
    currency2combo->blockSignals(true);

    currency1combo->clear();
    currency2combo->clear();

    int index = 0;
    for (auto i = currencies.cbegin(); i != currencies.cend(); i++) {
        QString currencyCode = i.key(), currencyName = i.value();
        QString displayString = QString("%1 (%2)").arg(currencyName, currencyCode);

        if (currencyName != "") {
            currency1combo->addItem(displayString, currencyCode);
            currency2combo->addItem(displayString, currencyCode);
        } else {
            continue;
        }

        if (currencyCode == "usd") {
            currency1combo->blockSignals(false);
            currency2combo->blockSignals(false);
            currency2combo->setCurrentIndex(index);
            currency1combo->setCurrentIndex(index);
            currency1combo->blockSignals(true);
            currency2combo->blockSignals(true);
        }

        index++;
    }

    currency1combo->blockSignals(false);
    currency2combo->blockSignals(false);
}

void MainWindow::updateExchange(const QString &baseCurrency, const QMap<QString, double> &rates)
{
    QComboBox *currency1combobox = findChild<QComboBox *>("currency1ComboBox");
    QComboBox *currency2combobox = findChild<QComboBox *>("currency2ComboBox");
    QDoubleSpinBox *currency1spinbox = findChild<QDoubleSpinBox *>("currency1SpinBox");

    currentRates = rates;

    updateCurrency2Spinbox(currency1spinbox->value() * currentRates[currency2combobox->currentData().toString()]);
    updateJumbotron(
        currency1combobox->currentData().toString(),
        currency1spinbox->value(),
        currency2combobox->currentData().toString(),
        currency1spinbox->value() * currentRates[currency2combobox->currentData().toString()]
        );
}

void MainWindow::networkError(const QString &error)
{
    qCritical() << error;
    QApplication::quit();
}

void MainWindow::on_currency1SpinBox_valueChanged(double arg1)
{
    QComboBox *currency1combobox = findChild<QComboBox *>("currency1ComboBox");
    QComboBox *currency2combobox = findChild<QComboBox *>("currency2ComboBox");

    updateCurrency2Spinbox(arg1 * currentRates[currency2combobox->currentData().toString()]);
    updateJumbotron(
        currency1combobox->currentData().toString(),
        arg1,
        currency2combobox->currentData().toString(),
        arg1 * currentRates[currency2combobox->currentData().toString()]
        );
}

void MainWindow::on_currency2ComboBox_currentIndexChanged(int index)
{
    QComboBox *currency1combobox = findChild<QComboBox *>("currency1ComboBox");
    QComboBox *currency2combobox = findChild<QComboBox *>("currency2ComboBox");
    QDoubleSpinBox *currency1spinbox = findChild<QDoubleSpinBox *>("currency1SpinBox");

    updateCurrency2Spinbox(currency1spinbox->value() * currentRates[currency2combobox->currentData().toString()]);
    updateJumbotron(
        currency1combobox->currentData().toString(),
        currency1spinbox->value(),
        currency2combobox->currentData().toString(),
        currency1spinbox->value() * currentRates[currency2combobox->currentData().toString()]
        );
}

void MainWindow::on_currency1ComboBox_currentIndexChanged(int index)
{
    QComboBox *currency1 = findChild<QComboBox *>("currency1ComboBox");
    apiManager->fetchCurrency(currency1->itemData(index).toString()); // We pass the data which is the currency code
}


void MainWindow::on_actionContact_triggered()
{
    // Create the dialog that pops up the credits / contact info
    creditDialog dialog(this);

    dialog.exec();
}


void MainWindow::on_actionParametres_triggered()
{
    SettingsDialog dialog(m_currentLang, this);

    if (dialog.exec() == QDialog::Accepted) {
        changeLanguage(dialog.getSelectedLanguage());
    }
}
