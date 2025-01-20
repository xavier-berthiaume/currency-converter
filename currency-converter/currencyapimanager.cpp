#include "currencyapimanager.h"

CurrencyAPIManager::CurrencyAPIManager(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &CurrencyAPIManager::handleNetworkReply);
}

void CurrencyAPIManager::fetchCurrencies()
{
    QString url = constructAPIUrl(CurrencyAPIManager::apiLatestDate,
                                    CurrencyAPIManager::apiVersion,
                                    "currencies.json");
    const QUrl apiUrl(url);
    QNetworkRequest request(apiUrl);

    QNetworkReply *reply = networkManager->get(request);
    requestTypeMap.insert(reply, CurrencyAPIManager::FetchCurrencies);
}

void CurrencyAPIManager::fetchCurrency(const QString &currency)
{
    QString url = constructAPIUrl(CurrencyAPIManager::apiLatestDate,
                                  CurrencyAPIManager::apiVersion,
                                  "currencies/" + currency + ".json");

    const QUrl apiUrl(url);
    QNetworkRequest request(apiUrl);

    QNetworkReply *reply = networkManager->get(request);
    requestTypeMap.insert(reply, CurrencyAPIManager::FetchExchangeRates);
}

QString CurrencyAPIManager::constructAPIUrl(const QString &date, const QString &version, const QString &endpoint)
{
    QString baseurl = "https://cdn.jsdelivr.net/npm/@fawazahmed0/currency-api@{date}/{apiVersion}/{endpoint}";
    baseurl.replace("{date}", date);
    baseurl.replace("{apiVersion}", version);
    baseurl.replace("{endpoint}", endpoint);

    return baseurl;
}

void CurrencyAPIManager::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error fetching currencies";
        emit errorFetchingCurrencies(reply->errorString());
        requestTypeMap.remove(reply);
        reply->deleteLater();
        return;
    }

    const QByteArray responseData = reply->readAll();
    QJsonDocument jsondocument = QJsonDocument::fromJson(responseData);

    if (!jsondocument.isObject()) {
        qWarning() << "Invalid JSON format";
        requestTypeMap.remove(reply);
        reply->deleteLater();
        return;
    }

    QJsonObject jsonobject = jsondocument.object();
    RequestType requestType = requestTypeMap.take(reply);

    if (requestType == FetchCurrencies) {
        QMap<QString, QString> currencies;
        for (auto currency = jsonobject.begin(); currency != jsonobject.end(); currency++) {
            currencies.insert(currency.key(), currency.value().toString());
        }
        emit currenciesFetched(currencies);
    } else if (requestType == FetchExchangeRates) {
        // Extract the base currency and date
        QStringList keys = jsonobject.keys();
        if (keys.size() < 2) {
            qWarning() << "Unexpected JSON structure: insufficient keys";
            reply->deleteLater();
            return;
        }

        QString basecurrency = keys[1];
        QJsonObject ratesObj = jsonobject[basecurrency].toObject();
        QMap<QString, double> rates;
        for (const QString &key : ratesObj.keys()) {
            rates.insert(key, ratesObj[key].toDouble());
        }
        emit exchangeRatesFetched(basecurrency, rates);
    }

    reply->deleteLater();
}
