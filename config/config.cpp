#include <QString>
#include <QSettings>

namespace subara {

namespace config {

bool is_authorized = false;
QString oauth_token = "";
QString oauth_token_secret = "";

void reset()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.clear();
}

} // end namespace config

} // end namespace subara
