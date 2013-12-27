#ifndef SUBARA_VIEWER_VIEWER_HPP
#define SUBARA_VIEWER_VIEWER_HPP

#include <QWebView>
#include <QWebFrame>
#include <QStyleOptionTitleBar>
#include <QStyle>
#include <QtAlgorithms>
#include <QDesktopServices>
#include <QWebElement>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include <QStatusBar>
#include "oauth/api/api.hpp"
#include "oauth/errmsg.hpp"

namespace subara {

class viewer : public QWebView
{
    Q_OBJECT

public:
    viewer(QMainWindow * parent = 0);

    void initialize();
    void initialize_layout();
    void initialize_base_hostname();
    void initialize_dashboard();

private:
    QMainWindow* super;

    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);

    bool loading_flag;
    unsigned int page_num;
    QString post_type;
    QString user_info;
    QString base_hostname;

    enum class post_type_enum : unsigned int
    {
        all   = 0u,
        text  = 1u,
        photo = 2u,
        quote = 3u,
        link  = 4u,
        chat  = 5u,
        audio = 6u,
        video = 7u
    };

private slots:
    void attachWindowObject();

public slots:
    void reload();
    void get_base_hostname(const QWebElement&);
    void load_next_page();
    void reblog();
    void reblog_impl(const QWebElement&, const QWebElement&, const QWebElement&);
    void like();
    void like_impl(const QWebElement&, const QWebElement&, const QWebElement&);
    void video_show_on_tumblr();
    void video_show_on_tumblr_impl(const QWebElement&, const QWebElement&);
    void audio_show_on_tumblr();
    void audio_show_on_tumblr_impl(const QWebElement&, const QWebElement&);
    void change_post_type(const unsigned int&);
};

} // end namespace subara

#endif // SUBARA_VIEWER_VIEWER_HPP
