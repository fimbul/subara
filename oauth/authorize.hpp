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

} // end namespace oauth

} // end namespace subara

#endif // SUBARA_OAUTH_AUTHORIZE_HPP
