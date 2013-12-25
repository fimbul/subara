#include "viewer.hpp"

namespace subara {

viewer::viewer(QWidget *parent)
    : QWebView(parent)
{
    initialize();
}

void viewer::initialize()
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
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

void viewer::video_show_on_tumblr(const QWebElement& url_args, const QWebElement& position_args)
{
    /* detect argument with the use of cursor and scroll position */

    QStyleOptionTitleBar so;
    so.titleBarState = 1;

    const auto scroll_pos = QCursor::pos().ry()
            + this->page()->mainFrame()->scrollPosition().y()
            - this->style()->pixelMetric(QStyle::PM_TitleBarHeight, &so, this);

    const auto urls = url_args.toPlainText().split("&");
    auto positions_s = position_args.toPlainText().split("&");

    //qDebug() << scroll_pos << urls << positions_s;

    QVector<int> positions;
    for (auto& elem : positions_s)
        positions.push_back(elem.toInt());
    positions.back() = scroll_pos + 1;

    const auto target = qLowerBound(positions.begin(), positions.end(), scroll_pos) - positions.begin() - 1;

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
        dashboard_data = oauth::api::dashboard();
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

    QVector<QString> initialize_dashboard_js = {
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
    #include "viewer/dashboard/dashboard.js.txt"
        ,
        "document.getElementById(\"dashboard\").innerHTML = dashboard;"
        ,
        "video_post_pos();"
    };

    for(auto& elem : initialize_dashboard_js)
    {
        this->page()->mainFrame()->evaluateJavaScript(elem);
    }
}

} // end namespace subara
