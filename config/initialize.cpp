#include "initialize.hpp"

namespace subara {

namespace config {

void initialize()
{
    initialize_value("config/is_authorized", subara::config::is_authorized);
    initialize_value("config/oauth_token", subara::config::oauth_token);
    initialize_value("config/oauth_token_secret", subara::config::oauth_token_secret);
}

} // end namespace config

} // end namespace subara
