#include "viewer.hpp"

namespace subara {

viewer::viewer(QWidget *parent)
    : QWebView(parent)
{
    initialize();
}

void viewer::initialize()
{
    initialize_layout();
    initialize_dashboard();
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
    #include "viewer/dashboard/dashboard.js.txt"
        ,
        "document.getElementById(\"dashboard\").innerHTML = dashboard;"
    };

    for(auto& elem : initialize_dashboard_js)
    {
        this->page()->mainFrame()->evaluateJavaScript(elem);
    }
}

} // end namespace subara
