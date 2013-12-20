#ifndef SUBARA_MAINWINDOW_HPP
#define SUBARA_MAINWINDOW_HPP

#include <QMainWindow>

namespace subara {

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);
};

} // end namespace subara

#endif // SUBARA_MAINWINDOW_HPP
