#ifndef ENROLLMENTTHREAD_H
#define ENROLLMENTTHREAD_H

#include <QThread>
#include <QString>
#include <QObject>

/**
 * @brief The EnrollmentThread class registers biometric features of new users
 */
class EnrollmentThread : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param path Path to the folder, which contains the train images
     * @param method Defines the feature extraction method (line-based, texture-based)
     */
    EnrollmentThread(QString path, int method);

    /**
     * @brief Overides QThread's method
     */
    void run();

signals:
    /**
     * @brief Signal to inform the UI about the progress
     */
    void enrollPercentageComplete(int);
    /**
     * @brief Signal to inform the UI about the elapsed time
     */
    void enrollTimeComplete(int,int);

public slots:
    /**
     * @brief Terminates the enrollment phase
     */
    void terminateThread();

private:
    int method;
    QString path;
    bool isCanceled = false;
};

#endif // ENROLLMENTTHREAD_H
