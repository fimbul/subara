#include "mainwindow.hpp"

namespace subara {

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), viewer(this)
{
    initialize();
}

void mainwindow::initialize()
{
    this->setCentralWidget(&viewer);

    initialize_view();

    viewer.show();
}

void mainwindow::initialize_view()
{
    viewer.setHtml(
                QString(
                #include "viewer/viewer.html.txt"
                    )
                );

    viewer.page()->mainFrame()->evaluateJavaScript(
                (QString("document.getElementsByTagName(\"style\")[0].innerHTML = \"")
                 +
                 QString(
                 #include "viewer/viewer.css.txt"
                     )
                 +
                 QString("\";")).simplified()
                );
}

} // end namespace subara
