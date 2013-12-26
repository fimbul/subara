#include "viewer.hpp"

namespace subara {

viewer::viewer(QWidget *parent)
    : QWebView(parent), loading_flag(false), page_num(0), post_type("None"), user_info(""), base_hostname("")
{
    initialize();
}

void viewer::initialize()
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanCloseWindows,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);

    this->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
    this->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    this->settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows,true);
    this->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,true);
    this->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);

    attachWindowObject();
    QObject::connect(this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachWindowObject()));

    try
    {
        user_info = oauth::api::info();
    }
    catch (const char* errmsg)
    {
        oauth::err_msg_alert(errmsg);
        return;
    }
    catch (const QString& errmsg)
    {
        oauth::err_msg_alert(errmsg);
        return;
    }
    catch (...)
    {
        oauth::err_msg_alert("Error: getting dashboard failed");
        return;
    }

    //qDebug() << user_info;
    initialize_layout();
    initialize_base_hostname();
    initialize_dashboard();
}

void viewer::initialize_base_hostname()
{
    this->page()->mainFrame()->evaluateJavaScript("var user_info = " + user_info + ";");
    this->page()->mainFrame()->evaluateJavaScript("document.getElementById(\"base_hostname\").innerHTML = user_info.response.user.blogs[0].url;");
    this->page()->mainFrame()->evaluateJavaScript("cppapi['get_base_hostname(const QWebElement&)'](document.getElementById('base_hostname'));");
}

void viewer::get_base_hostname(const QWebElement& arg)
{
    base_hostname = arg.toPlainText().remove(QRegExp("(http://|/)"));
    //qDebug() << arg.toPlainText();
}

void viewer::attachWindowObject()
{
    this->page()->mainFrame()->addToJavaScriptWindowObject(QString("cppapi"), this);
}

void viewer::video_show_on_tumblr()
{
    this->page()->mainFrame()->evaluateJavaScript("video_post_pos();");
    this->page()->mainFrame()->evaluateJavaScript("cppapi['video_show_on_tumblr_impl(const QWebElement&, const QWebElement&)'](document.getElementById('video_post_argument'), document.getElementById('video_post_poslist'))");
}

void viewer::video_show_on_tumblr_impl(const QWebElement& url_args, const QWebElement& position_args)
{
    /* detect argument with the use of cursor and scroll position */

    QStyleOptionTitleBar so;
    so.titleBarState = 1;

    const auto click_pos = QCursor::pos().ry() - this->style()->pixelMetric(QStyle::PM_TitleBarHeight, &so, this);

    const auto urls = url_args.toPlainText().split("&");
    auto positions_s = position_args.toPlainText().split("&");

    QVector<int> positions;
    for (auto& elem : positions_s)
        positions.push_back(elem.toInt());
    positions.pop_back();

    //qDebug() << click_pos << urls << positions;

    const auto target = qLowerBound(positions.begin(), positions.end(), click_pos) - positions.begin() - 1;

    if (target < urls.size())
    {
        QDesktopServices::openUrl(QUrl(urls.at(target)));
    }
};

void viewer::audio_show_on_tumblr()
{
    this->page()->mainFrame()->evaluateJavaScript("audio_post_pos();");
    this->page()->mainFrame()->evaluateJavaScript("cppapi['audio_show_on_tumblr_impl(const QWebElement&, const QWebElement&)'](document.getElementById('audio_post_argument'), document.getElementById('audio_post_poslist'))");
}

void viewer::audio_show_on_tumblr_impl(const QWebElement& url_args, const QWebElement& position_args)
{
    /* detect argument with the use of cursor and scroll position */

    QStyleOptionTitleBar so;
    so.titleBarState = 1;

    const auto click_pos = QCursor::pos().ry() - this->style()->pixelMetric(QStyle::PM_TitleBarHeight, &so, this);

    const auto urls = url_args.toPlainText().split("&");
    auto positions_s = position_args.toPlainText().split("&");

    QVector<int> positions;
    for (auto& elem : positions_s)
        positions.push_back(elem.toInt());
    positions.pop_back();

    //qDebug() << click_pos << urls << positions;

    const auto target = qLowerBound(positions.begin(), positions.end(), click_pos) - positions.begin() - 1;

    if (target < urls.size())
    {
        QDesktopServices::openUrl(QUrl(urls.at(target)));
    }
};

void viewer::reblog()
{
    this->page()->mainFrame()->evaluateJavaScript("post_pos();");
    this->page()->mainFrame()->evaluateJavaScript("cppapi['reblog_impl(const QWebElement&, const QWebElement&, const QWebElement&)'](document.getElementById('post_id'), document.getElementById('reblog_key'), document.getElementById('post_poslist'))");
}

void viewer::reblog_impl(const QWebElement& post_id_args, const QWebElement& reblog_key_args, const QWebElement& position_args)
{
    /* detect argument with the use of cursor and scroll position */
    QStyleOptionTitleBar so;
    so.titleBarState = 1;

    const auto click_pos = QCursor::pos().ry() - this->style()->pixelMetric(QStyle::PM_TitleBarHeight, &so, this);

    const auto post_ids = post_id_args.toPlainText().split("&");
    const auto reblog_keys = reblog_key_args.toPlainText().split("&");
    auto positions_s = position_args.toPlainText().split("&");

    QVector<int> positions;
    for (auto& elem : positions_s)
        positions.push_back(elem.toInt());
    positions.pop_back();

    //qDebug() << click_pos << urls << positions;

    const auto target = qLowerBound(positions.begin(), positions.end(), click_pos) - positions.begin() - 1;

    if (target < reblog_keys.size())
    {
        QString reblog_key = reblog_keys.at(target);
        QString post_id = post_ids.at(target);
        //qDebug() << base_hostname << post_id << reblog_key;
        qDebug() << oauth::api::reblog(base_hostname, reblog_key, post_id);
    }
}

void viewer::initialize_layout()
{
    this->setHtml(
                QString(
                #include "viewer/viewer.html.txt"
                    )
                );

    this->page()->mainFrame()->evaluateJavaScript(
                (QString("document.getElementById(\"viewer_css\").innerHTML = \"")
                 +
                 QString(
                 #include "viewer/viewer.css.txt"
                     )
                 +
                 QString("\";")).simplified()
                );
}

void viewer::initialize_dashboard()
{
    QString dashboard_data = "";

    try
    {
        dashboard_data = oauth::api::dashboard(config::api::limit, 0, post_type);
    }
    catch (const char* errmsg)
    {
        oauth::err_msg_alert(errmsg);
        return;
    }
    catch (const QString& errmsg)
    {
        oauth::err_msg_alert(errmsg);
        return;
    }
    catch (...)
    {
        oauth::err_msg_alert("Error: getting dashboard failed");
        return;
    }

    QStringList initialize_dashboard_js = {
        "var post_data = \"\";",
        "var posts = " + dashboard_data + ".response.posts;",
    #include "viewer/dashboard/photo_post.js.txt"
        ,
    #include "viewer/dashboard/quote_post.js.txt"
        ,
    #include "viewer/dashboard/text_post.js.txt"
        ,
    #include "viewer/dashboard/link_post.js.txt"
        ,
    #include "viewer/dashboard/video_post.js.txt"
        ,
    #include "viewer/dashboard/audio_post.js.txt"
        ,
    #include "viewer/dashboard/chat_post.js.txt"
        ,
    #include "viewer/dashboard/post_pos.js.txt"
    };

    for (auto& elem : initialize_dashboard_js)
    {
        this->page()->mainFrame()->evaluateJavaScript(elem);
    }

    load_next_page();
}

void viewer::load_next_page()
{
    ++page_num;

    QString dashboard_data = "";

    try
    {
        dashboard_data = oauth::api::dashboard(config::api::limit, config::api::limit * (page_num - 1), post_type);
    }
    catch (const char* errmsg)
    {
        oauth::err_msg_alert(errmsg);
        return;
    }
    catch (const QString& errmsg)
    {
        oauth::err_msg_alert(errmsg);
        return;
    }
    catch (...)
    {
        oauth::err_msg_alert("Error: getting dashboard failed");
        return;
    }

    QStringList initialize_dashboard_js = {
        "var post_data = \"\";",
        "var posts = " + dashboard_data + ".response.posts;",
    #include "viewer/dashboard/dashboard.js.txt"
        ,
        "var page_data = document.createElement('div');",
        "page_data.id = \"page",
        QString::number(page_num),
        "\";",
        "page_data.innerHTML = post_data;",
        "document.getElementById(\"dashboard\").appendChild(page_data);"
    };

    for (auto& elem : initialize_dashboard_js)
    {
        this->page()->mainFrame()->evaluateJavaScript(elem);
    }
}

void viewer::wheelEvent(QWheelEvent* event)
{
    QWebView::wheelEvent(event);

    if (!loading_flag)
    {
        loading_flag = true;
        if (this->page()->mainFrame()->scrollPosition().y() == this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical))
        {
            load_next_page();
        }
        loading_flag = false;
    }
}

void viewer::keyPressEvent(QKeyEvent* event)
{
    QWebView::keyPressEvent(event);

    if (!loading_flag)
    {
        loading_flag = true;
        const auto key = event->key();
        if (key == Qt::Key_Down || key == Qt::Key_PageDown || key == Qt::Key_Space)
        {
            if (this->page()->mainFrame()->scrollPosition().y() == this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical))
            {
                load_next_page();
            }
        }
        loading_flag = false;
    }
}

} // end namespace subara
