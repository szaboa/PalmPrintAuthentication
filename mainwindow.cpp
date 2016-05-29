#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QSize>
#include <QMessageBox>
#include <QImage>
#include "EnrollmentThread.h"
#include <utility/QtUtils.h>
#include <QProgressDialog>
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
    ui->selectedEnrollmentFolderLabel->setText(enrollmentFolder);
    ui->selectedAuthenticationFolderLabel->setText(authFolder);


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

void MainWindow::initLogger(){
    // Load configuration from file
    el::Configurations conf("/home/szabo/Documents/PPA/PPA/easylogging/conf");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
}

void MainWindow::on_selectEnrollmentFolderButton_clicked()
{
    enrollmentFolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/szabo/Documents/PPA/PPA", QFileDialog::ShowDirsOnly);
    ui->selectedEnrollmentFolderLabel->setText(enrollmentFolder);
}

void MainWindow::on_selectAuthFolderButton_clicked()
{
    authFolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/szabo/Documents/PPA/PPA", QFileDialog::ShowDirsOnly);
    ui->selectedAuthenticationFolderLabel->setText(authFolder);
}

QString MainWindow::getEnrollmentFolder(){
    return enrollmentFolder;
}

QString MainWindow::getAuthFolder(){
    return authFolder;
}

QString MainWindow::getSingleVerificationPath(){
    return singleVerificationPath;
}

void MainWindow::on_startEnrollmentButton_clicked()
{
    QString selectedFolder = getEnrollmentFolder();
    if(selectedFolder.isEmpty()){
        QtUtils::showInfoMessage("You need to select a folder, before starting the enrollment phase.");
    }else{
        enrollmentThread = new EnrollmentThread(selectedFolder, getSelectedMethod());
        enrollmentThread->start();

        LOG(INFO) << "Start EnrollmentThread";

        enrollmentProgressDialog = new QProgressDialog("Enrollment...", "Cancel",0,100,this);
        enrollmentProgressDialog->setWindowModality(Qt::WindowModal);
        connect(enrollmentThread, SIGNAL(enrollPercentageComplete(int)),this,SLOT(displayEnrollmentProgress(int)));
        connect(enrollmentThread, SIGNAL(enrollTimeComplete(int,int)),this,SLOT(displayTimeMeasurments(int,int)));
    }
}

void MainWindow::displayTimeMeasurments(int total, int avg){
    ui->label_enTotalTime->setText(QString::number(total) + " ms");
    ui->label_enAvgTime->setText(QString::number(avg)+ " ms");
}

void MainWindow::displayEnrollmentProgress(int value){
    enrollmentProgressDialog->setValue(value);
}

void MainWindow::displayAuthTimeMeasurments(int total, int avg){
    ui->label_auTotalTime->setText(QString::number(total) + " ms");
    ui->label_auAvgTime->setText(QString::number(avg)+ " ms");
}

void MainWindow::displayAuthProgress(int value){
    authProgressDialog->setValue(value);
}

void MainWindow::displayAuthMatchScore(double score){
    LOG(INFO) << score;
    ui->label_auMatchRate->setText(QString::number(score) + "%");
}

void MainWindow::displayFeature(cv::Mat featureMat){
    QImage image = QtUtils::matToQImage(featureMat);
    image = image.scaledToWidth(ui->labelFeatures->width(), Qt::SmoothTransformation);
    ui->labelFeatures->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::displayRoi(cv::Mat roiMat){
    QImage image = QtUtils::matToQImage(roiMat);
    image = image.scaledToWidth(ui->labelRoi->width(), Qt::SmoothTransformation);
    ui->labelRoi->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::displayMatchedFeature(double matchedDistance, int matchedId, cv::Mat matchedFeature){
    QImage image = QtUtils::matToQImage(matchedFeature);
    image = image.scaledToWidth(ui->labelMatchedFeature->width(), Qt::SmoothTransformation);
    ui->labelMatchedFeature->setPixmap(QPixmap::fromImage(image));
    ui->labelMatchedId->setText("Matched User Id: " + QString::number(matchedId));
    ui->labelDistance->setText("Distance: " + QString::number(matchedDistance));
}

void MainWindow::displayQueryImageId(int userId){
    ui->labelQueryImageId->setText("User Id: "  + QString::number(userId));
}

int MainWindow::getSelectedMethod(){
    int method = -1;
    if(this->ui->radioBtnLine->isChecked()){
        return QtUtils::LINE_METHOD;
    }else if(this->ui->radioBtnTexture->isChecked()){
        return QtUtils::TEXTURE_METHOD;
    }
}

void MainWindow::on_startAuthButton_clicked()
{

    QString selectedFolder = getAuthFolder();
    if(selectedFolder.isEmpty()){
        QtUtils::showInfoMessage("You need to select a folder, before starting the authentication phase.");
    }else{
        authThread = new AuthenticationThread(selectedFolder, getSelectedMethod());
        authThread->start();

        LOG(INFO) << "Start EnrollmentThread";

        authProgressDialog = new QProgressDialog("Authentication...", "Cancel",0,100,this);
        authProgressDialog->setWindowModality(Qt::WindowModal);
        connect(authThread, SIGNAL(authPercentageComplete(int)),this,SLOT(displayAuthProgress(int)));
        connect(authThread, SIGNAL(authTimeComplete(int,int)),this,SLOT(displayAuthTimeMeasurments(int,int)));
        connect(authThread, SIGNAL(authMatchComplete(double)),this,SLOT(displayAuthMatchScore(double)));


    }
}

void MainWindow::on_pushButton_clicked()
{
    singleVerificationPath = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images (*.png *.jpg *.jpeg *.JPG)"));
    if(!singleVerificationPath.isEmpty()){
        QImage inputImage;
        bool isValid = inputImage.load(singleVerificationPath);

        if(isValid){
            inputImage = inputImage.scaledToWidth(ui->label_input_image->width(), Qt::SmoothTransformation);
            ui->label_input_image->setPixmap(QPixmap::fromImage(inputImage));
        }else{
            QtUtils::showInfoMessage("Error: corrupted file.");
            singleVerificationPath = "";
        }
    }
}

void MainWindow::on_startVerificationButton_clicked()
{

    if(getSingleVerificationPath().isEmpty()){
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
