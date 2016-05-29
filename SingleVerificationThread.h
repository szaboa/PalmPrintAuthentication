#ifndef SINGLEVERIFICATION_H
#define SINGLEVERIFICATION_H

#include <QThread>
#include <QString>
#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class SingleVerificationThread: public QThread
{
public:
    Q_OBJECT

public:
   SingleVerificationThread(QString path, int method);
   void run();
signals:
    void sendQueryImageId(int id);
    void sendExtractedRoi(cv::Mat roi);
    void sendExtractedFeature(cv::Mat feature);
    void sendMatchedFeature(double matchedDistance, int matchedId, cv::Mat feature);

private:
   int method;
   QString path;
};

#endif // SINGLEVERIFICATION_H
