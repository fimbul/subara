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

QString dashboard(
        unsigned int limit,
        unsigned long long int offset,
        QString type,
        bool reblog_info,
        bool notes_info,
        int since_id
        )
{
    QString oauth_request_url = "";
    const QString oauth_api_url = "http://api.tumblr.com/v2/user/dashboard";
    const QString oauth_nonce = QString::number(qrand());
    const QString oauth_signature_key = QString(QUrl::toPercentEncoding(subara::config::oauth_secret_key))
            + '&'
            + QString(QUrl::toPercentEncoding(oauth_token_secret));
    const QString oauth_timestamp = QString::number(std::chrono::seconds(std::time(0)).count());
    const QString limit_s = QString::number(limit);
    const QString offset_s = QString::number(offset);
    const QString since_id_s = QString::number(since_id);
    const QString reblog_info_s = QString::number(reblog_info);
    const QString notes_info_s = QString::number(notes_info);

    QStringList oauth_signature_base_string_list;
    oauth_signature_base_string_list << ("limit=" + limit_s)
                                     << ("notest_info=" + notes_info_s)
                                     << ("oauth_consumer_key=" + oauth_consumer_key)
                                     << ("oauth_nonce=" + oauth_nonce)
                                     << ("oauth_signature_method=" + oauth_signature_method)
                                     << ("oauth_timestamp=" + oauth_timestamp)
                                     << ("oauth_token=" + oauth_token)
                                     << ("oauth_version=" + oauth_version)
                                     << ("offset=" + offset_s)
                                     << ("reblog_info=" + reblog_info_s);
    if(since_id > 0)
    {
        oauth_signature_base_string_list << ("since_id=" + since_id_s);
    }
    if(type != "None")
    {
        oauth_signature_base_string_list << ("type=" + type);
    }

    const QString oauth_signature_base_string = "GET&"
            + QUrl::toPercentEncoding(oauth_api_url) + "&"
            + QUrl::toPercentEncoding(oauth_signature_base_string_list.join("&"));

    const QString oauth_signature = QUrl::toPercentEncoding(
                QMessageAuthenticationCode::hash(
                    oauth_signature_base_string.toLatin1(),
                    oauth_signature_key.toLatin1(),
                    QCryptographicHash::Sha1).toBase64()
                );

    QStringList oauth_request_url_list;
    oauth_request_url_list << ("limit=" + limit_s)
                           << ("notest_info=" + notes_info_s)
                           << ("oauth_consumer_key=" + oauth_consumer_key)
                           << ("oauth_nonce=" + oauth_nonce)
                           << ("oauth_signature_method=" + oauth_signature_method)
                           << ("oauth_signature=" + oauth_signature)
                           << ("oauth_timestamp=" + oauth_timestamp)
                           << ("oauth_token=" + oauth_token)
                           << ("oauth_version=" + oauth_version)
                           << ("offset=" + offset_s)
                           << ("reblog_info=" + reblog_info_s);
    if(since_id > 0)
    {
        oauth_request_url_list << ("since_id=" + since_id_s);
    }
    if(type != "None")
    {
        oauth_request_url_list << ("type=" + type);
    }

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
        //qDebug() << oauth_request_reply_string;

        return oauth_request_reply_string;
    }
    else
    {
        const QString oauth_error_msg = oauth_request_reply->errorString();
        qDebug() << oauth_error_msg;
        throw ("Error : failed to get request token\n\nDetail : \n" + oauth_error_msg);
    }
}

QString info()
{
    QString oauth_request_url = "";
    const QString oauth_api_url = "http://api.tumblr.com/v2/user/info";
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

    const QString oauth_signature_base_string = "GET&"
            + QUrl::toPercentEncoding(oauth_api_url) + "&"
            + QUrl::toPercentEncoding(oauth_signature_base_string_list.join("&"));

    const QString oauth_signature = QUrl::toPercentEncoding(
                QMessageAuthenticationCode::hash(
                    oauth_signature_base_string.toLatin1(),
                    oauth_signature_key.toLatin1(),
                    QCryptographicHash::Sha1).toBase64()
                );

    QStringList oauth_request_url_list;
    oauth_request_url_list << ("oauth_consumer_key=" + oauth_consumer_key)
                           << ("oauth_nonce=" + oauth_nonce)
                           << ("oauth_signature_method=" + oauth_signature_method)
                           << ("oauth_signature=" + oauth_signature)
                           << ("oauth_timestamp=" + oauth_timestamp)
                           << ("oauth_token=" + oauth_token)
                           << ("oauth_version=" + oauth_version);

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
        //qDebug() << oauth_request_reply_string;

        return oauth_request_reply_string;
    }
    else
    {
        const QString oauth_error_msg = oauth_request_reply->errorString();
        qDebug() << oauth_error_msg;
        throw ("Error : failed to get request token\n\nDetail : \n" + oauth_error_msg);
    }
}

bool reblog(const QString& base_hostname, const QString& reblog_key, const QString& id)
{
    const QString oauth_request_url = "http://api.tumblr.com/v2/blog/" + base_hostname + "/post/reblog";
    const QString oauth_nonce = QString::number(qrand());
    const QString oauth_signature_key = QString(QUrl::toPercentEncoding(subara::config::oauth_secret_key))
            + '&'
            + QString(QUrl::toPercentEncoding(oauth_token_secret));
    const QString oauth_timestamp = QString::number(std::chrono::seconds(std::time(0)).count());

    QStringList oauth_signature_base_string_list;
    oauth_signature_base_string_list << ("id=" + id)
                                     << ("oauth_consumer_key=" + oauth_consumer_key)
                                     << ("oauth_nonce=" + oauth_nonce)
                                     << ("oauth_signature_method=" + oauth_signature_method)
                                     << ("oauth_timestamp=" + oauth_timestamp)
                                     << ("oauth_token=" + oauth_token)
                                     << ("oauth_version=" + oauth_version)
                                     << ("reblog_key=" + reblog_key);

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

    qDebug() << base_hostname << reblog_key << id << oauth_signature;

    QNetworkAccessManager oauth_network_manager;
    QEventLoop oauth_request_event_loop;

    QObject::connect(&oauth_network_manager, SIGNAL(finished(QNetworkReply*)), &oauth_request_event_loop, SLOT(quit()));

    QNetworkRequest request(oauth_request_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(QString("Authorization").toUtf8(), authorization_header.toLatin1());

    QNetworkReply* oauth_request_reply = oauth_network_manager.post(request, (("id=" + id) + ("&reblog_key=" + reblog_key)).toLatin1());
    oauth_request_event_loop.exec();

    if (oauth_request_reply->error() == QNetworkReply::NoError)
    {
        const QString oauth_request_reply_string = oauth_request_reply->readAll();
        qDebug() << oauth_request_reply_string;
        return true;
    }
    else
    {
        const QString oauth_request_reply_string = oauth_request_reply->readAll();
        qDebug() << oauth_request_reply_string;
        const QString oauth_error_msg = oauth_request_reply->errorString();
        qDebug() << oauth_error_msg;
        return false;
    }
}

} // end namespace api

} // end namespace oauth

} // end namespace subara
