#include "authorize.hpp"

namespace subara {

namespace oauth {

void authorize(const QApplication& app)
{
    /* [TASK] implement "retry" button */
    try
    {
        authorize_impl(app);
    }
    catch (const char* errmsg)
    {
        err_msg_alert(errmsg);
        throw;
    }
    catch (const QString& errmsg)
    {
        err_msg_alert(errmsg);
        throw;
    }
    catch (...)
    {
        err_msg_alert("Authorization Error");
        throw;
    }
}

void authorize_impl(const QApplication& app)
{
    const QString oauth_consumer_key = subara::config::oauth_consumer_key;
    const QString oauth_secret_key = subara::config::oauth_secret_key;
    const QString oauth_request_token_url = "http://www.tumblr.com/oauth/request_token";
    const QString oauth_authorize_url = "http://www.tumblr.com/oauth/authorize";
    const QString oauth_access_token_url = "http://www.tumblr.com/oauth/access_token";
    const QString oauth_signature_method = "HMAC-SHA1";
    const QString oauth_nonce = QString::number(qrand());
    const QString oauth_version = "1.0";
    const QString oauth_callback = QUrl::toPercentEncoding("https://github.com/fimbul");

    QString oauth_token = "";
    QString oauth_token_secret = "";
    QString oauth_verifier = "";

    // get request token
    {
        QString oauth_request_url = "";
        const QString oauth_signature_key = QString(QUrl::toPercentEncoding(subara::config::oauth_secret_key)) + '&';
        const QString oauth_timestamp = QString::number(std::chrono::seconds(std::time(0)).count());

        QStringList oauth_signature_base_string_list;
        oauth_signature_base_string_list << ("oauth_callback=" + oauth_callback)
                                         << ("oauth_consumer_key=" + oauth_consumer_key)
                                         << ("oauth_nonce=" + oauth_nonce)
                                         << ("oauth_signature_method=" + oauth_signature_method)
                                         << ("oauth_timestamp=" + oauth_timestamp)
                                         << ("oauth_version=" + oauth_version);

        const QString oauth_signature_base_string = "GET&"
                + QUrl::toPercentEncoding(oauth_request_token_url) + "&"
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
                         << ("oauth_version=" + oauth_version)
                         << ("oauth_callback=" + oauth_callback);

        oauth_request_url = oauth_request_token_url + "?" + oauth_request_url_list.join("&");


        QNetworkAccessManager oauth_network_manager;
        QEventLoop oauth_request_event_loop;

        QObject::connect(&oauth_network_manager, SIGNAL(finished(QNetworkReply*)), &oauth_request_event_loop, SLOT(quit()));

        QNetworkReply* oauth_request_reply = oauth_network_manager.get(QNetworkRequest(oauth_request_url));
        oauth_request_event_loop.exec();


        if (oauth_request_reply->error() == QNetworkReply::NoError)
        {
            const QString oauth_request_reply_string = oauth_request_reply->readAll();
            QStringList oauth_request_reply_list = oauth_request_reply_string.split(QRegExp("(=|&)"));

            if (oauth_request_reply_list.size() >= 4)
            {
                oauth_token = oauth_request_reply_list.at(1);
                oauth_token_secret = oauth_request_reply_list.at(3);
            }
            else
            {
                throw "Error : failed to get request token";
            }
        }
        else
        {
            const QString oauth_error_msg = oauth_request_reply->errorString();
            throw ("Error : failed to get request token\n\nDetail : \n" + oauth_error_msg);
        }
    } // end get request token

    // get oauth_verifier
    {
        const QString oauth_request_url = oauth_authorize_url + "?oauth_token=" + oauth_token;

        QWebView oauth_web_view;
        oauth_web_view.load(QUrl(oauth_request_url));
        oauth_web_view.show();

        QString oauth_verifier_url = "";
        QEventLoop oauth_request_event_loop;
        QObject::connect(&oauth_web_view, SIGNAL(loadFinished(bool)), &oauth_request_event_loop, SLOT(quit()));
        do {
            oauth_request_event_loop.exec();

            oauth_verifier_url = oauth_web_view.url().toEncoded();
            const auto oauth_tmp_string_list = oauth_verifier_url.split(QRegExp("(oauth_verifier=|#_=_)"));

            if(oauth_tmp_string_list.size() >= 2)
                oauth_verifier = oauth_tmp_string_list.at(1);
        } while (!oauth_verifier_url.contains("oauth_verifier=") && oauth_web_view.isVisible());

        oauth_web_view.close();

        if(oauth_verifier == "")
        {
            throw "Error : authorization failed";
        }
    } // end get oauth_verifier

    // get access token
    {
        QString oauth_request_url = "";
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
                                         << ("oauth_verifier=" + oauth_verifier)
                                         << ("oauth_version=" + oauth_version);

        const QString oauth_signature_base_string = "GET&"
                + QUrl::toPercentEncoding(oauth_access_token_url) + "&"
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
                               << ("oauth_verifier=" + oauth_verifier)
                               << ("oauth_version=" + oauth_version);

        oauth_request_url = oauth_access_token_url + "?" + oauth_request_url_list.join("&");

        QNetworkAccessManager oauth_network_manager;
        QEventLoop oauth_request_event_loop;

        QObject::connect(&oauth_network_manager, SIGNAL(finished(QNetworkReply*)), &oauth_request_event_loop, SLOT(quit()));

        QNetworkReply* oauth_request_reply = oauth_network_manager.get(QNetworkRequest(oauth_request_url));
        oauth_request_event_loop.exec();


        if (oauth_request_reply->error() == QNetworkReply::NoError)
        {
            const QString oauth_request_reply_string = oauth_request_reply->readAll();
            QStringList oauth_request_reply_list = oauth_request_reply_string.split(QRegExp("(=|&)"));

            if (oauth_request_reply_list.size() >= 4)
            {
                oauth_token = oauth_request_reply_list.at(1);
                oauth_token_secret = oauth_request_reply_list.at(3);
            }
            else
            {
                throw "Error : failed to get access token";
            }
        }
        else
        {
            const QString oauth_error_msg = oauth_request_reply->errorString();
            throw ("Error : failed to get access token\n\nDetail : \n" + oauth_error_msg);
        }

        QSettings settings(app.applicationDirPath() + QDir::separator() + "settings.ini", QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.setValue("config/oauth_token", oauth_token);
        settings.setValue("config/oauth_token_secret", oauth_token_secret);
        settings.setValue("config/is_authorized", true);

        subara::config::oauth_token = oauth_token;
        subara::config::oauth_token_secret = oauth_token_secret;
        subara::config::is_authorized = true;
    } // end get access token
}

} // end namespace oauth

} // end namespace subara
