#ifndef CURRENCYAPIMANAGER_H
#define CURRENCYAPIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class CurrencyAPIManager : public QObject
{
    Q_OBJECT

    enum RequestType {
        FetchCurrencies,
        FetchExchangeRates
    };

    const QString apiVersion = "v1";
    const QString apiLatestDate = "latest";

    QNetworkAccessManager *networkManager;
    QHash<QNetworkReply *, RequestType> requestTypeMap;

    // Github page for the API: https://github.com/fawazahmed0/exchange-api
    // Request url structure is detailed in the README
    QString constructAPIUrl(const QString &date,
                            const QString &version,
                            const QString &endpoint);

public:
    explicit CurrencyAPIManager(QObject *parent = nullptr);

    void fetchCurrencies();
    void fetchCurrency(const QString &currency);

signals:
    void currenciesFetched(const QMap<QString, QString> &currencies);
    void exchangeRatesFetched(const QString &baseCurrency, const QMap<QString, double> &rates);
    void errorFetchingCurrencies(const QString &error);

private slots:
    void handleNetworkReply(QNetworkReply *reply);
};

#endif // CURRENCYAPIMANAGER_H
