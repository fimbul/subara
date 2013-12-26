#ifndef SUBARA_MAINWINDOW_HPP
#define SUBARA_MAINWINDOW_HPP

#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>
#include <QCloseEvent>
#include <QSettings>
#include "viewer/viewer.hpp"

namespace subara {

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);

private:
    viewer viewer;
    void initialize();
};

} // end namespace subara

#endif // SUBARA_MAINWINDOW_HPP
