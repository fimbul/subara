#ifndef SUBARA_OAUTH_API_HPP
#define SUBARA_OAUTH_API_HPP

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <chrono>
#include "config/config.hpp"

namespace subara {

namespace oauth {

namespace api {

extern const QString& oauth_signature_method;
extern const QString& oauth_version;
extern const QString& oauth_consumer_key;
extern const QString& oauth_secret_key;
extern const QString& oauth_token;
extern const QString& oauth_token_secret;

QString dashboard(
        unsigned int limit = 20,
        unsigned long long int offset = 0,
        QString type = "None",
        bool reblog_info = false,
        bool notes_info = false,
        int since_id = -1
        );

QString info();

bool reblog(const QString&, const QString&, const QString&);
bool like(const QString&, const QString&);

} // end namespace api

} // end namespace oauth

} // end namespace subara

#endif // SUBARA_OAUTH_API_HPP
