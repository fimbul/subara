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
    }
    catch (const QString& errmsg)
    {
        oauth::err_msg_alert(errmsg);
    }
    catch (...)
    {
        oauth::err_msg_alert("Error: getting dashboard failed");
    }

    QVector<QString> initialize_dashboard_js = {
        "var dashboard = \"\";",
        "var posts = " + dashboard_data + ".response.posts;",
        "function photo_post(post) { post.photos.forEach(function(photo){ dashboard += \"<img src=\\\"\" + photo.alt_sizes[0].url + \"\\\" /><br />\"; }); }",
        "posts.forEach(function(post){ if (post.type == \"photo\") { photo_post(post); } });",
        "document.getElementById(\"dashboard\").innerHTML = dashboard;"
    };

    for(auto& elem : initialize_dashboard_js)
    {
        this->page()->mainFrame()->evaluateJavaScript(elem);
    }
}

} // end namespace subara
