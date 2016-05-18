#ifndef AUTHENTICATIONTHREAD_H
#define AUTHENTICATIONTHREAD_H

#include <QThread>
#include <QString>
#include <QObject>

class AuthenticationThread : public QThread
{
     Q_OBJECT

public:
    AuthenticationThread(QString path, int method);
    void run();
signals:
    void authPercentageComplete(int);
    void authTimeComplete(int,int);
    void authMatchComplete(double);
private:
    int method;
    QString path;
};

#endif // AUTHENTICATIONTHREAD_H
