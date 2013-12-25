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
#include "oauth/api/api.hpp"
#include "oauth/errmsg.hpp"

namespace subara {

class viewer : public QWebView
{
    Q_OBJECT

public:
    viewer(QWidget *parent = 0);

    void initialize();
    void initialize_layout();
    void initialize_dashboard();

private:
    void wheelEvent(QWheelEvent*);
    bool scroll_flag;

    void keyPressEvent(QKeyEvent*);
    bool keypress_flag;

    unsigned int page_num;
    QString post_type;

private slots:
    void attachWindowObject();

public slots:
    void load_next_page();
    void video_show_on_tumblr();
    void video_show_on_tumblr_impl(const QWebElement&, const QWebElement&);
    void audio_show_on_tumblr();
    void audio_show_on_tumblr_impl(const QWebElement&, const QWebElement&);
};

} // end namespace subara

#endif // SUBARA_VIEWER_VIEWER_HPP
