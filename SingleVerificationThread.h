#ifndef SINGLEVERIFICATION_H
#define SINGLEVERIFICATION_H

#include <QThread>
#include <QString>
#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The SingleVerificationThread class performs a single user indentification (1:N)
 */
class SingleVerificationThread: public QThread
{
public:
    Q_OBJECT

public:
    /**
    * @brief Constructor
    * @param path Path to the query image
    * @param method Defines the feature extraction method (line-based, texture-based)
    */
   SingleVerificationThread(QString path, int method);

   /**
    * @brief overrides QThread's method
    */
   void run();

signals:

   /**
     * @brief Signal to inform the UI about the user id of the selected query image
     * @param User id which belongs to the selected query image
     */
    void sendQueryImageId(int id);

    /**
     * @brief Signal to pass the extracted palm area from the selected query image
     * @param Extracted palm area
     */
    void sendExtractedRoi(cv::Mat roi);


    /**
     * @brief Signal to pass the extracted feature from the palm area
     * @param Extracted feature from the palm area
     */
    void sendExtractedFeature(cv::Mat feature);

    /**
     * @brief Signal to pass the matched feature
     * @param Matching distance between the extracted feature of the query image and the matched image
     * @param Id of the matched user
     * @param Image of the matched feature
     */
    void sendMatchedFeature(double matchedDistance, int matchedId, cv::Mat feature);

private:
   int method;
   QString path;
};

#endif // SINGLEVERIFICATION_H
