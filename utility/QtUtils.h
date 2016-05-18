#ifndef QTUTILS_H
#define QTUTILS_H

#include <QString>
#include <QMessageBox>

class QtUtils
{
public:
    QtUtils();
    static void showInfoMessage(QString message){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(message);
        msgBox.exec();
    }

    static const int LINE_METHOD = 1;
    static const int TEXTURE_METHOD = 2;
};


#endif // QTUTILS_H
