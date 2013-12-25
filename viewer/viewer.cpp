#include "viewer.hpp"

namespace subara {

viewer::viewer(QWidget *parent)
    : QWebView(parent), page_num(0), scroll_flag(false)
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

    initialize_layout();
    initialize_dashboard();
}

void viewer::attachWindowObject()
{
    this->page()->mainFrame()->addToJavaScriptWindowObject(QString("cppapi"), this);
};

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

    qDebug() << click_pos << urls << positions;

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

    qDebug() << click_pos << urls << positions;

    const auto target = qLowerBound(positions.begin(), positions.end(), click_pos) - positions.begin() - 1;

    if (target < urls.size())
    {
        QDesktopServices::openUrl(QUrl(urls.at(target)));
    }
};

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
        dashboard_data = oauth::api::dashboard(config::api::limit);
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
        "var dashboard = \"\";",
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
        dashboard_data = oauth::api::dashboard(config::api::limit, config::api::limit * (page_num - 1));
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
        "var posts = " + dashboard_data + ".response.posts;",
    #include "viewer/dashboard/dashboard.js.txt"
        ,
        "document.getElementById(\"dashboard\").innerHTML = dashboard;"
    };

    for (auto& elem : initialize_dashboard_js)
    {
        this->page()->mainFrame()->evaluateJavaScript(elem);
    }
}

void viewer::wheelEvent(QWheelEvent* event)
{
    if (!scroll_flag)
    {
        scroll_flag = true;
        QWebView::wheelEvent(event);
        if (this->page()->mainFrame()->scrollPosition().y() == this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical))
        {
            load_next_page();
        }
        scroll_flag = false;
    }
}

} // end namespace subara
