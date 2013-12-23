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
    viewer.show();
}

} // end namespace subara
