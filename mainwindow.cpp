#include "mainwindow.hpp"

namespace subara {

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), viewer(this)
{
    initialize();
}

void mainwindow::initialize()
{
    this->statusBar()->showMessage("");
    this->setCentralWidget(&viewer);
    viewer.show();
}

} // end namespace subara
