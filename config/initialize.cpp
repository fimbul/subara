#include "initialize.hpp"

namespace subara {

namespace config {

bool is_authorized = false;
QString oauth_token = "";
QString oauth_token_secret = "";

void initialize()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariant setting_v = settings.value("config/is_authorized");
    if (setting_v.type() != QVariant::Invalid)
    {
        qDebug() << setting_v.toBool();
        subara::config::is_authorized = setting_v.toBool();
    }

    setting_v = settings.value("config/oauth_token");
    if (setting_v.type() != QVariant::Invalid)
    {
        qDebug() << setting_v.toString();
        oauth_token = setting_v.toString();
    }

    setting_v = settings.value("config/oauth_token_secret");
    if (setting_v.type() != QVariant::Invalid)
    {
        qDebug() << setting_v.toString();
        oauth_token_secret = setting_v.toString();
    }
}

void reset()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.clear();
}

} // end namespace config

} // end namespace subara
