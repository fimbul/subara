#include <QApplication>
#include "config/config.hpp"
#include "config/initialize.hpp"
#include "oauth/authorize.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication subara(argc, argv);

    subara::config::initialize(subara);

    if (!subara::config::is_authorized)
    {
        try
        {
            subara::oauth::authorize(subara);
        }
        catch (...)
        {
            QApplication::quit();
            exit(EXIT_FAILURE);
        }
    }

    subara::mainwindow mainwindow;
    mainwindow.showMaximized();

    subara.exec();
}
