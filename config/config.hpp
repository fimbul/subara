#ifndef SUBARA_CONFIG_CONFIG_HPP
#define SUBARA_CONFIG_CONFIG_HPP

#include <QString>

namespace subara {

namespace config {

static constexpr auto oauth_consumer_key = "L72Uiqid518klGE3QMDFc4E80p0KKQHpKHBauHmZrcHTPnKxjV";
static constexpr auto oauth_secret_key = "x3rZTUEUEPXfler9h7WnrxoHyn4dU77LsR9giaUAmFtMi5CZUj";

extern bool is_authorized;
extern QString oauth_token;
extern QString oauth_token_secret;

namespace api {

  extern unsigned int limit;

} // end namespace api

void reset();

} // end namespace config

} // end namespace subara

#endif // SUBARA_CONFIG_CONFIG_HPP
