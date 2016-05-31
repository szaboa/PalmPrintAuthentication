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

/**
 * @brief The MainWindow class sets up the UI and provides an interface to interract with it
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /**
     * @brief Displays enrollment phase progress
     * @param Progress percentage
     */
    void displayEnrollmentProgress(int value);


    /**
     * @brief Displays time information about the enrollment phase
     * @param Total time taken of the enrollment phase
     * @param Avarage time taken to enroll one user
     */
    void displayEnrollmentTimeMeasurments(int total, int avg);

    /**
     * @brief Displays authentication phase progress
     * @param Progress percetange
     */
    void displayAuthProgress(int value);

    /**
     * @brief Displays time information about the authentication phase
     * @param Total time taken of the authentication phase
     * @param Avarage time taken to verify one user
     */
    void displayAuthTimeMeasurments(int total, int avg);

    /**
     * @brief Displays the successfuly identified rate
     * @param Score (0-100)
     */
    void displayAuthMatchScore(double score);

    /**
     * @brief Displays the extracted palm area (ROI)
     * @param Image of the palm area (ROI)
     */
    void displayRoi(cv::Mat roi);

    /**
     * @brief Displays the extracted feature from the palm area (ROI)
     * @param Image of the extracted feature
     */
    void displayFeature(cv::Mat feature);

    /**
     * @brief Displaying the matched feature for the QueryImage's extracted feature
     * @param Matching distance between the extracted feature of the query image and the matched image
     * @param Id of the matched user
     * @param Image of the matched feature
     */
    void displayMatchedFeature(double matchedDistance, int matchedId, cv::Mat matchedFeature);

    /**
     * @brief Displays the query image
     * @param User id which belongs to the query image
     */
    void displayQueryImageId(int userId);

    void on_buttonSelectQueryImage_clicked();

    void on_buttonStartVerification_clicked();

    void on_buttonSelectEnrollmentFolder_clicked();

    void on_buttonStartEnrollment_clicked();

    void on_buttonSelectAuthFolder_clicked();

    void on_buttonStartAuthFolder_clicked();

private:
    Ui::MainWindow *ui;

    /**
     * @brief Path to the train images
     */
    QString enrollDatabasePath = "/home/szabo/Documents/PPA/PPA/_train_data";

    /**
     * @brief Path to the test images
     */
    QString authDatabasePath = "/home/szabo/Documents/PPA/PPA/_test_data";

    /**
     * @brief Path to the query image
     */
    QString singleVerificationPath;

    AuthenticationThread* authThread = nullptr;
    EnrollmentThread* enrollmentThread = nullptr;
    SingleVerificationThread* singleVerificationThread = nullptr;

    QProgressDialog* enrollmentProgressDialog = nullptr;
    QProgressDialog* authProgressDialog = nullptr;

    QString getEnrollDatabasePath();
    QString getAuthDatabasePath();
    QString getSingleVerificationPath();

    int getSelectedMethod();

    /**
     * @brief Initializes the logger library
     */
    void initLogger();

    /**
     * @brief Initializes the labels on the enrollment tab
     */
    void initEnrollmentTab();

    /**
     * @brief Initializes the labels on the authentication tab
     */
    void initAuthTab();
};

#endif // MAINWINDOW_H
