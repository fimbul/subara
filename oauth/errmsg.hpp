#ifndef SUBARA_OAUTH_ERRMSG_HPP
#define SUBARA_OAUTH_ERRMSG_HPP

#include <QMessageBox>

namespace subara {

namespace oauth {

template <class T>
void err_msg_alert(const T& errmsg)
{
    QMessageBox errmsg_box;
    errmsg_box.setText(errmsg);
    errmsg_box.exec();
}

} // end namespace oauth

} // end namespace subara

#endif // SUBARA_OAUTH_ERRMSG_HPP
