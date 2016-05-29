#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "EnrollmentThread.h"
#include "AuthenticationThread.h"
#include "SingleVerificationThread.h"
#include <QProgressDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectEnrollmentFolderButton_clicked();

    void on_startEnrollmentButton_clicked();

    void displayEnrollmentProgress(int value);

    void displayTimeMeasurments(int total, int avg);

    void displayAuthProgress(int value);

    void displayAuthTimeMeasurments(int total, int avg);

    void displayAuthMatchScore(double score);

    void displayRoi(cv::Mat roi);

    void displayFeature(cv::Mat feature);

    void displayMatchedFeature(double matchedDistance, int matchedId, cv::Mat matchedFeature);

    void displayQueryImageId(int userId);

    QString getEnrollmentFolder();
    QString getAuthFolder();
    QString getSingleVerificationPath();

    void on_selectAuthFolderButton_clicked();

    void on_startAuthButton_clicked();

    void on_pushButton_clicked();

    void on_startVerificationButton_clicked();

private:
    Ui::MainWindow *ui;
    QString enrollmentFolder = "/home/szabo/Documents/PPA/PPA/_train_data";
    QString authFolder = "/home/szabo/Documents/PPA/PPA/_test_data";
    QString singleVerificationPath;

    AuthenticationThread* authThread = nullptr;
    EnrollmentThread* enrollmentThread = nullptr;
    SingleVerificationThread* singleVerificationThread = nullptr;
    QProgressDialog* enrollmentProgressDialog = nullptr;
    QProgressDialog* authProgressDialog = nullptr;

    int getSelectedMethod();
    void initLogger();
};

#endif // MAINWINDOW_H
