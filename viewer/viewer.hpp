#ifndef SUBARA_VIEWER_VIEWER_HPP
#define SUBARA_VIEWER_VIEWER_HPP

#include <QWebView>
#include <QWebFrame>
#include "oauth/api/api.hpp"
#include "oauth/errmsg.hpp"

namespace subara {

class viewer : public QWebView
{
    Q_OBJECT

public:
    viewer(QWidget *parent = 0);

private:
    void initialize();
    void initialize_layout();
    void initialize_dashboard();
};

} // end namespace subara

#endif // SUBARA_VIEWER_VIEWER_HPP
