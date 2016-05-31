#ifndef AUTHENTICATIONTHREAD_H
#define AUTHENTICATIONTHREAD_H

#include <QThread>
#include <QString>
#include <QObject>

/**
 * @brief The AuthenticationThread class performs verification on each test image (N:N)
 */
class AuthenticationThread : public QThread
{
     Q_OBJECT

public:
    /**
     * @brief Constuctor
     * @param path Path to the folder which contains the test images
     * @param method Defines the feature extraction method (line-based, texture-based)
     */
    AuthenticationThread(QString path, int method);

    /**
     * @brief Overides QThread's method
     */
    void run();
signals:
    /**
     * @brief Signal to inform the UI about the progress
     */
    void authPercentageComplete(int);

    /**
     * @brief Signal to inform the UI about the elapsed time
     */
    void authTimeComplete(int,int);

    /**
     * @brief Signal to inform the UI about the matching score
     */
    void authMatchComplete(double);

public slots:
    /**
     * @brief Terminates the authentication phase
     */
    void terminateThread();

private:
    int method;
    QString path;
    bool isCanceled = false;
};

#endif // AUTHENTICATIONTHREAD_H
