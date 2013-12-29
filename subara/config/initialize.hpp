#ifndef SUBARA_CONFIG_INITIALIZE_HPP
#define SUBARA_CONFIG_INITIALIZE_HPP

#include <QDebug>
#include <QVariant>
#include <QString>
#include <QDir>
#include <QApplication>
#include <QSettings>
#include "config.hpp"

namespace subara {

namespace config {

void initialize(const QApplication&);

template<class T>
static void initialize_value(const QString& appdir, const char* name, T& target)
{
    static QSettings settings(appdir + QDir::separator() + "settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    static QVariant setting_v;
    setting_v = settings.value(name);
    if (setting_v.type() != QVariant::Invalid && setting_v.canConvert<T>())
    {
        target = setting_v.value<T>();
    }
}


} // end namespace config

} // end namespace subara

#endif // SUBARA_CONFIG_INITIALIZE_HPP
