#ifndef SUBARA_CONFIG_INITIALIZE_HPP
#define SUBARA_CONFIG_INITIALIZE_HPP

#include <QDebug>
#include <QVariant>
#include <QString>
#include <QSettings>
#include "config.hpp"

namespace subara {

namespace config {

void initialize();

template<class T>
static void initialize_value(const char* name, T& target)
{
    static QSettings settings("settings.ini", QSettings::IniFormat);
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
