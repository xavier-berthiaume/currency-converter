#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    left->setText(QString::number(value1, 'f', 2) + " " + currency1);
    right->setText(QString::number(value2, 'f', 2) + " " + currency2);
}

void MainWindow::updateCurrency2Spinbox(const double &value)
{
    QDoubleSpinBox *currency2spin = findChild<QDoubleSpinBox *>("currency2SpinBox");

    currency2spin->setValue(value);
}

void MainWindow::updateCurrencies(const QMap<QString, QString> &currencies)
{
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

