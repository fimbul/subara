#ifndef SUBARA_MAINWINDOW_HPP
#define SUBARA_MAINWINDOW_HPP

#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>

namespace subara {

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);

private:
    QWebView viewer;
    void initialize();
    void initialize_view();
};

} // end namespace subara

#endif // SUBARA_MAINWINDOW_HPP
