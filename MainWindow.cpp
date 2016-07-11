#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "EnrollmentThread.h"

#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QSize>
#include <QMessageBox>
#include <QImage>
#include <QProgressDialog>

#include <utility/QtUtils.h>
#include <easylogging++.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

INITIALIZE_EASYLOGGINGPP

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("cv::Mat");

    // Init paths to the train and test image folders
    ui->labelEnrollDatabasePathValue->setText(enrollDatabasePath);
    ui->labelAuthDatabasePathValue->setText(authDatabasePath);

    initLogger();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete enrollmentThread;
    delete enrollmentProgressDialog;
    delete authThread;
    delete authProgressDialog;
}

void MainWindow::initLogger()
{
    // Load configuration from file
    el::Configurations conf("/home/szabo/Documents/PPA/PPA/easylogging/conf");

    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
}


QString MainWindow::getEnrollDatabasePath()
{
    return enrollDatabasePath;
}

QString MainWindow::getAuthDatabasePath()
{
    return authDatabasePath;
}

QString MainWindow::getSingleVerificationPath()
{
    return singleVerificationPath;
}

void MainWindow::initAuthTab()
{
    ui->labelAuthAvgTimeValue->setText("0");
    ui->labelAuthMatchRateValue->setText("0");
    ui->labelAuthTotalTimeValue->setText("0");
}

void MainWindow::initEnrollmentTab()
{
    ui->labelEnrollAvgTimeValue->setText("0");
    ui->labelEnrollTotalTimeValue->setText("0");
}

void MainWindow::displayEnrollmentTimeMeasurments(int total, int avg)
{
    ui->labelEnrollTotalTimeValue->setText(QString::number(total) + " ms");
    ui->labelEnrollAvgTimeValue->setText(QString::number(avg)+ " ms");
}

void MainWindow::displayAuthTimeMeasurments(int total, int avg)
{
    ui->labelAuthTotalTimeValue->setText(QString::number(total) + " ms");
    ui->labelAuthAvgTimeValue->setText(QString::number(avg)+ " ms");
}


void MainWindow::displayEnrollmentProgress(int value)
{
    if(enrollmentProgressDialog->isVisible())
    {
        enrollmentProgressDialog->setValue(value);
    }
}

void MainWindow::displayAuthProgress(int value)
{
    if(authProgressDialog->isVisible())
    {
        authProgressDialog->setValue(value);
    }
}

void MainWindow::displayAuthMatchScore(double score)
{
    ui->labelAuthMatchRateValue->setText(QString::number(score) + "%");
}

void MainWindow::displayRoi(cv::Mat roiMat)
{
    QImage image = QtUtils::matToQImage(roiMat);
    image = image.scaledToWidth(ui->imgRoi->width(), Qt::SmoothTransformation);
    ui->imgRoi->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::displayFeature(cv::Mat featureMat)
{
    QImage image = QtUtils::matToQImage(featureMat);
    image = image.scaledToWidth(ui->imgExtractedFeature->width(), Qt::SmoothTransformation);
    ui->imgExtractedFeature->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::displayMatchedFeature(double matchedDistance, int matchedId, cv::Mat matchedFeature)
{
    QImage image = QtUtils::matToQImage(matchedFeature);
    image = image.scaledToWidth(ui->labelMatchedFeature->width(), Qt::SmoothTransformation);
    ui->imgMatchedFeature->setPixmap(QPixmap::fromImage(image));
    ui->labelMatchedId->setText("Matched User Id: " + QString::number(matchedId));
    ui->labelDistance->setText("Distance: " + QString::number(matchedDistance));

    if(getSelectedMethod() == QtUtils::LINE_METHOD){
        if(matchedDistance < 0.011){
            this->ui->labelDecision->setText("<font color='green'>ACCEPT</font>");
        }else{
            this->ui->labelDecision->setText("<font color='red'>REJECT</font>");
        }
    }else if(getSelectedMethod() == QtUtils::TEXTURE_METHOD){
        if(matchedDistance < 0.137){
            this->ui->labelDecision->setText("<font color='green'>ACCEPT</font>");
        }else{
            this->ui->labelDecision->setText("<font color='red'>REJECT</font>");
        }
    }
}

void MainWindow::displayQueryImageId(int userId)
{
    ui->labelQueryImageId->setText("User Id: "  + QString::number(userId));
}

int MainWindow::getSelectedMethod()
{
    if(this->ui->radioButtonLine->isChecked())
    {
        return QtUtils::LINE_METHOD;
    }else if(this->ui->radioButtonTexture->isChecked())
    {
        return QtUtils::TEXTURE_METHOD;
    }
}

void MainWindow::on_buttonSelectEnrollmentFolder_clicked()
{
    enrollDatabasePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/szabo/Documents/PPA/PPA", QFileDialog::ShowDirsOnly);
    ui->labelEnrollDatabasePathValue->setText(enrollDatabasePath);
}

void MainWindow::on_buttonStartEnrollment_clicked()
{
    QString selectedFolder = getEnrollDatabasePath();
    if(selectedFolder.isEmpty())
    {
        QtUtils::showInfoMessage("You need to select a folder, before starting the enrollment phase.");
    }else{
        initEnrollmentTab();
        enrollmentThread = new EnrollmentThread(selectedFolder, getSelectedMethod());
        enrollmentThread->start();


        enrollmentProgressDialog = new QProgressDialog("Enrollment...", "Cancel",0,100,this);
        enrollmentProgressDialog->setWindowModality(Qt::WindowModal);
        enrollmentProgressDialog->show();

        connect(enrollmentThread, SIGNAL(enrollPercentageComplete(int)),this,SLOT(displayEnrollmentProgress(int)));
        connect(enrollmentThread, SIGNAL(enrollTimeComplete(int,int)),this,SLOT(displayEnrollmentTimeMeasurments(int,int)));
        connect(enrollmentProgressDialog,SIGNAL(canceled()),enrollmentThread,SLOT(terminateThread()));
    }
}

void MainWindow::on_buttonSelectAuthFolder_clicked()
{
    authDatabasePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/szabo/Documents/PPA/PPA", QFileDialog::ShowDirsOnly);
    ui->labelAuthDatabasePathValue->setText(authDatabasePath);
}

void MainWindow::on_buttonStartAuthFolder_clicked()
{
    QString selectedFolder = getAuthDatabasePath();
    if(selectedFolder.isEmpty())
    {
        QtUtils::showInfoMessage("You need to select a folder, before starting the authentication phase.");
    }else{
        initAuthTab();
        authThread = new AuthenticationThread(selectedFolder, getSelectedMethod());
        authThread->start();

        authProgressDialog = new QProgressDialog("Authentication...", "Cancel",0,100,this);
        authProgressDialog->setWindowModality(Qt::WindowModal);
        authProgressDialog->show();

        connect(authThread, SIGNAL(authPercentageComplete(int)),this,SLOT(displayAuthProgress(int)));
        connect(authThread, SIGNAL(authTimeComplete(int,int)),this,SLOT(displayAuthTimeMeasurments(int,int)));
        connect(authThread, SIGNAL(authMatchComplete(double)),this,SLOT(displayAuthMatchScore(double)));
        connect(authProgressDialog,SIGNAL(canceled()),authThread,SLOT(terminateThread()));
    }
}

void MainWindow::on_buttonSelectQueryImage_clicked()
{
    singleVerificationPath = QFileDialog::getOpenFileName(this,tr("Choose"),"/home/szabo/Documents/PPA/PPA",tr("Images (*.png *.jpg *.jpeg *.JPG)"));
    if(!singleVerificationPath.isEmpty())
    {
        QImage inputImage;
        bool isValid = inputImage.load(singleVerificationPath);

        if(isValid)
        {
            inputImage = inputImage.scaledToWidth(ui->imgQueryImage->width(), Qt::SmoothTransformation);
            ui->imgQueryImage->setPixmap(QPixmap::fromImage(inputImage));
        }else{
            QtUtils::showInfoMessage("Error: corrupted file.");
            singleVerificationPath = "";
        }
    }
}

void MainWindow::on_buttonStartVerification_clicked()
{
    if(getSingleVerificationPath().isEmpty())
    {
        QtUtils::showInfoMessage("You need to choose an input image.");
    }else{
        singleVerificationThread = new SingleVerificationThread(getSingleVerificationPath(), getSelectedMethod());
        singleVerificationThread->start();

        connect(singleVerificationThread,SIGNAL(sendExtractedRoi(cv::Mat)),this, SLOT(displayRoi(cv::Mat)));
        connect(singleVerificationThread,SIGNAL(sendExtractedFeature(cv::Mat)),this, SLOT(displayFeature(cv::Mat)));
        connect(singleVerificationThread,SIGNAL(sendMatchedFeature(double,int,cv::Mat)),this, SLOT(displayMatchedFeature(double,int,cv::Mat)));
        connect(singleVerificationThread,SIGNAL(sendQueryImageId(int)),this, SLOT(displayQueryImageId(int)));
    }
}
