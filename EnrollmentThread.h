#ifndef ENROLLMENTTHREAD_H
#define ENROLLMENTTHREAD_H

#include <QThread>
#include <QString>
#include <QObject>

class EnrollmentThread : public QThread
{
    Q_OBJECT
public:
    EnrollmentThread(QString path, int method);
    void run();
signals:
    void enrollPercentageComplete(int);
    void enrollTimeComplete(int,int);
private:
    int method;
    QString path;
};

#endif // ENROLLMENTTHREAD_H
