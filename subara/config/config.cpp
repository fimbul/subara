#include <QString>
#include <QSettings>
#include <QApplication>
#include <QDir>

namespace subara {

namespace config {

bool is_authorized = false;
QString oauth_token = "";
QString oauth_token_secret = "";

namespace api {

unsigned int limit = 20;

} // end namespace api

void reset(const QApplication& app)
{
    QSettings settings(app.applicationDirPath() + QDir::separator() + "settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.clear();
}

} // end namespace config

} // end namespace subara
