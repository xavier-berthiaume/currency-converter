#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::updateJumbotron(const QString &leftLabel, const QString &rightLabel)
{
    QLabel *left = findChild<QLabel *>("currency1Label");
    QLabel *right = findChild<QLabel *>("currency2Label");

    left->setText(leftLabel);
    right->setText(rightLabel);
}

void MainWindow::updateCurrencies(const QMap<QString, QString> &currencies)
{
    QComboBox *currency1combo = findChild<QComboBox *>("currency1ComboBox");
    QComboBox *currency2combo = findChild<QComboBox *>("currency2ComboBox");

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
            currency1combo->setCurrentIndex(index);
            currency2combo->setCurrentIndex(index);
        }

        index++;
    }
}

void MainWindow::updateExchange(const QString &baseCurrency, const QMap<QString, double> &rates)
{

}

void MainWindow::networkError(const QString &error)
{
    qCritical() << error;
    QApplication::quit();
}

void MainWindow::on_currency1ComboBox_currentIndexChanged(int index)
{

}


void MainWindow::on_currency2ComboBox_currentIndexChanged(int index)
{

}


void MainWindow::on_currency1SpinBox_valueChanged(double arg1)
{

}


void MainWindow::on_currency2SpinBox_valueChanged(double arg1)
{

}

