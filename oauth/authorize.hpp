#ifndef SUBARA_OAUTH_AUTHORIZE_HPP
#define SUBARA_OAUTH_AUTHORIZE_HPP

#include "mainwindow.hpp"
#include "config/config.hpp"
#include <QtWebKitWidgets>
#include <memory>
#include <chrono>

namespace subara {

namespace oauth {

void authorize_impl();
void authorize();

template <class T>
void err_msg_alert(const T& errmsg)
{
    QMessageBox errmsg_box;
    errmsg_box.setText(errmsg);
    errmsg_box.exec();
}

} // end namespace oauth

} // end namespace subara

#endif // SUBARA_OAUTH_AUTHORIZE_HPP
