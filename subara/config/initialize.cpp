#include "initialize.hpp"

namespace subara {

namespace config {

void initialize(const QApplication& app)
{
    const auto appdir = app.applicationDirPath();
    initialize_value(appdir, "config/is_authorized", subara::config::is_authorized);
    initialize_value(appdir, "config/oauth_token", subara::config::oauth_token);
    initialize_value(appdir, "config/oauth_token_secret", subara::config::oauth_token_secret);
}

} // end namespace config

} // end namespace subara
