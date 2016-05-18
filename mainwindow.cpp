#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "EnrollmentThread.h"
#include <utility/QtUtils.h>
#include <QProgressDialog>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

void MainWindow::on_startEnrollmentButton_clicked()
{

    int method = -1;
    if(this->ui->radioBtnLine->isChecked()){
        method = QtUtils::LINE_METHOD;
    }else if(this->ui->radioBtnTexture->isChecked()){
        method = QtUtils::TEXTURE_METHOD;
    }

    QString selectedFolder = getEnrollmentFolder();
    if(selectedFolder.isEmpty()){
        QtUtils::showInfoMessage("You need to select a folder, before starting the enrollment phase.");
    }else{
        enrollmentThread = new EnrollmentThread(selectedFolder, method);
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

void MainWindow::on_startAuthButton_clicked()
{

    int method = -1;
    if(this->ui->radioBtnLine->isChecked()){
        method = QtUtils::LINE_METHOD;
    }else if(this->ui->radioBtnTexture->isChecked()){
        method = QtUtils::TEXTURE_METHOD;
    }

    QString selectedFolder = getAuthFolder();
    if(selectedFolder.isEmpty()){
        QtUtils::showInfoMessage("You need to select a folder, before starting the authentication phase.");
    }else{
        authThread = new AuthenticationThread(selectedFolder, method);
        authThread->start();

        LOG(INFO) << "Start EnrollmentThread";

        authProgressDialog = new QProgressDialog("Authentication...", "Cancel",0,100,this);
        authProgressDialog->setWindowModality(Qt::WindowModal);
        connect(authThread, SIGNAL(authPercentageComplete(int)),this,SLOT(displayAuthProgress(int)));
        connect(authThread, SIGNAL(authTimeComplete(int,int)),this,SLOT(displayAuthTimeMeasurments(int,int)));
        connect(authThread, SIGNAL(authMatchComplete(double)),this,SLOT(displayAuthMatchScore(double)));

    }
}
