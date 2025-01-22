#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

#include "currencyapimanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;

    CurrencyAPIManager *apiManager;

    QMap<QString, double> currentRates;

    void calculateExchange();
    void updateJumbotron(const QString &currency1, const double &value1, const QString &currency2, const double &value2);
    void updateCurrency2Spinbox(const double &value);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateCurrencies(const QMap<QString, QString> &currencies);
    void updateExchange(const QString &baseCurrency, const QMap<QString, double> &rates);

    void networkError(const QString &error);
    void on_currency2ComboBox_currentIndexChanged(int index);
    void on_currency1ComboBox_currentIndexChanged(int index);
    void on_currency1SpinBox_valueChanged(double arg1);
    void on_actionContact_triggered();
};
#endif // MAINWINDOW_H
