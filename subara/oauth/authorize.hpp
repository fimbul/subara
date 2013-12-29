#ifndef SUBARA_OAUTH_AUTHORIZE_HPP
#define SUBARA_OAUTH_AUTHORIZE_HPP

#include "mainwindow.hpp"
#include "config/config.hpp"
#include "errmsg.hpp"
#include <QtWebKitWidgets>
#include <QApplication>
#include <chrono>

namespace subara {

namespace oauth {

void authorize_impl(const QApplication&);
void authorize(const QApplication&);

} // end namespace oauth

} // end namespace subara

#endif // SUBARA_OAUTH_AUTHORIZE_HPP
