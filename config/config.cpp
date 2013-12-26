#include <QString>
#include <QSettings>

namespace subara {

namespace config {

bool is_authorized = false;
QString oauth_token = "";
QString oauth_token_secret = "";

namespace api {

unsigned int limit = 20;

} // end namespace api

void reset()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.clear();
}

} // end namespace config

} // end namespace subara
