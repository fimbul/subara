#include "api.hpp"

namespace subara {

namespace oauth {

namespace api {

const QString& oauth_signature_method = "HMAC-SHA1";
const QString& oauth_version = "1.0";
const QString& oauth_consumer_key = subara::config::oauth_consumer_key;
const QString& oauth_secret_key = subara::config::oauth_secret_key;
const QString& oauth_token = subara::config::oauth_token;
const QString& oauth_token_secret = subara::config::oauth_token_secret;

QPair<bool, QString> oauth_get(const QString& oauth_api_url, const QVector<QString>& params)
{
    QString oauth_request_url = "";
    const QString oauth_nonce = QString::number(qrand());
    const QString oauth_signature_key = QString(QUrl::toPercentEncoding(subara::config::oauth_secret_key))
            + '&'
            + QString(QUrl::toPercentEncoding(oauth_token_secret));
    const QString oauth_timestamp = QString::number(std::chrono::seconds(std::time(0)).count());

    QStringList oauth_signature_base_string_list;
    oauth_signature_base_string_list << ("oauth_consumer_key=" + oauth_consumer_key)
                                     << ("oauth_nonce=" + oauth_nonce)
                                     << ("oauth_signature_method=" + oauth_signature_method)
                                     << ("oauth_timestamp=" + oauth_timestamp)
                                     << ("oauth_token=" + oauth_token)
                                     << ("oauth_version=" + oauth_version);

    for(auto& elem : params)
        oauth_signature_base_string_list << elem;

    oauth_signature_base_string_list.sort();

    const QString oauth_signature_base_string = "GET&"
            + QUrl::toPercentEncoding(oauth_api_url) + "&"
            + QUrl::toPercentEncoding(oauth_signature_base_string_list.join("&"));

    const QString oauth_signature = QUrl::toPercentEncoding(
                QMessageAuthenticationCode::hash(
                    oauth_signature_base_string.toLatin1(),
                    oauth_signature_key.toLatin1(),
                    QCryptographicHash::Sha1).toBase64()
                );

    QStringList& oauth_request_url_list = oauth_signature_base_string_list;
    oauth_request_url_list << ("oauth_signature=" + oauth_signature);

    oauth_request_url = oauth_api_url + "?" + oauth_request_url_list.join("&");
    //qDebug() << oauth_request_url;

    QNetworkAccessManager oauth_network_manager;
    QEventLoop oauth_request_event_loop;

    QObject::connect(&oauth_network_manager, SIGNAL(finished(QNetworkReply*)), &oauth_request_event_loop, SLOT(quit()));

    QNetworkReply* oauth_request_reply = oauth_network_manager.get(QNetworkRequest(oauth_request_url));
    oauth_request_event_loop.exec();

    if (oauth_request_reply->error() == QNetworkReply::NoError)
    {
        const QString oauth_request_reply_string = oauth_request_reply->readAll();

        return qMakePair(true, oauth_request_reply_string);
    }
    else
    {
        const QString oauth_error_msg = oauth_request_reply->errorString();
        throw ("Error : failed to request\n\nDetail : \n" + oauth_error_msg);
    }
}

QPair<bool, QString> oauth_post(const QString& oauth_request_url, const QVector<QString>& params)
{
    const QString oauth_nonce = QString::number(qrand());
    const QString oauth_signature_key = QString(QUrl::toPercentEncoding(subara::config::oauth_secret_key))
            + '&'
            + QString(QUrl::toPercentEncoding(oauth_token_secret));
    const QString oauth_timestamp = QString::number(std::chrono::seconds(std::time(0)).count());

    QStringList oauth_signature_base_string_list;
    oauth_signature_base_string_list << ("oauth_consumer_key=" + oauth_consumer_key)
                                     << ("oauth_nonce=" + oauth_nonce)
                                     << ("oauth_signature_method=" + oauth_signature_method)
                                     << ("oauth_timestamp=" + oauth_timestamp)
                                     << ("oauth_token=" + oauth_token)
                                     << ("oauth_version=" + oauth_version);

    for(auto& elem : params)
        oauth_signature_base_string_list << elem;

    oauth_signature_base_string_list.sort();

    const QString oauth_signature_base_string = "POST&"
            + QUrl::toPercentEncoding(oauth_request_url) + "&"
            + QUrl::toPercentEncoding(oauth_signature_base_string_list.join("&"));

    const QString oauth_signature = QUrl::toPercentEncoding(
                QMessageAuthenticationCode::hash(
                    oauth_signature_base_string.toLatin1(),
                    oauth_signature_key.toLatin1(),
                    QCryptographicHash::Sha1).toBase64()
                );

    QStringList authorization_header_list;
    authorization_header_list << ("oauth_consumer_key=" + oauth_consumer_key)
                              << ("oauth_nonce=" + oauth_nonce)
                              << ("oauth_signature=" + oauth_signature)
                              << ("oauth_signature_method=" + oauth_signature_method)
                              << ("oauth_timestamp=" + oauth_timestamp)
                              << ("oauth_token=" + oauth_token)
                              << ("oauth_version=" + oauth_version);
   const auto authorization_header = "OAuth " + authorization_header_list.join(",");

    QNetworkAccessManager oauth_network_manager;
    QEventLoop oauth_request_event_loop;

    QObject::connect(&oauth_network_manager, SIGNAL(finished(QNetworkReply*)), &oauth_request_event_loop, SLOT(quit()));

    QNetworkRequest request(oauth_request_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(QString("Authorization").toUtf8(), authorization_header.toUtf8());

    QStringList params_str;
    for(auto& elem : params)
        params_str << elem;

    QNetworkReply* oauth_request_reply = oauth_network_manager.post(request, (params_str.join("&")).toUtf8());
    oauth_request_event_loop.exec();

    if (oauth_request_reply->error() == QNetworkReply::NoError)
    {
        const QString oauth_request_reply_string = oauth_request_reply->readAll();
        return qMakePair(true, oauth_request_reply_string);
    }
    else
    {
        const QString oauth_error_msg = oauth_request_reply->errorString();
        return qMakePair(false, oauth_error_msg);
    }
}

QString dashboard(
        unsigned int limit,
        unsigned long long int offset,
        QString type,
        bool reblog_info,
        bool notes_info,
        int since_id
        )
{
    QVector<QString> params;

    params.push_back("limit=" + QString::number(limit));
    params.push_back("offset=" + QString::number(offset));
    params.push_back("reblog_info=" + QString::number(reblog_info));
    params.push_back("notes_info=" + QString::number(notes_info));

    if (since_id > 0)
    {
        params.push_back("sinze_id=" + QString::number(since_id));
    }
    if (type != "all")
    {
        params.push_back("type=" + type);
    }

    return oauth_get("http://api.tumblr.com/v2/user/dashboard", params).second;
}

QString info()
{
    return oauth_get("http://api.tumblr.com/v2/user/info").second;
}

bool reblog(const QString& base_hostname, const QString& reblog_key, const QString& id)
{
    QVector<QString> params;
    params.push_back("reblog_key=" + reblog_key);
    params.push_back("id=" + id);

    return oauth_post("http://api.tumblr.com/v2/blog/" + base_hostname + "/post/reblog", params).first;
}

bool like(const QString& reblog_key, const QString& id)
{
    QVector<QString> params;
    params.push_back("reblog_key=" + reblog_key);
    params.push_back("id=" + id);

    return oauth_post("http://api.tumblr.com/v2/user/like", params).first;
}

QString user_likes(unsigned int limit, unsigned long long int offset)
{
    QVector<QString> params;

    params.push_back("limit=" + QString::number(limit));
    params.push_back("offset=" + QString::number(offset));

    return oauth_get("http://api.tumblr.com/v2/user/likes", params).second;
}

} // end namespace api

} // end namespace oauth

} // end namespace subara
