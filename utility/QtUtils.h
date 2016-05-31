#ifndef QTUTILS_H
#define QTUTILS_H

#include <QString>
#include <QMessageBox>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
/**
 * @brief The QtUtils class is a utility function collection related to Qt
 */
class QtUtils
{
public:
    QtUtils();

    /**
     * @brief Shows an info dialog with the given message
     * @param message Info message
     */
    static void showInfoMessage(QString message){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(message);
        msgBox.exec();
    }

    /**
     * @brief Converts cv::Mat object to QImage
     * @param mat cv::Mat object
     * @return QImage object
     */
    static QImage matToQImage(const Mat& mat)
    {
        // 8-bits unsigned, NO. OF CHANNELS=1
        if(mat.type()==CV_8UC1)
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for (int i=0; i<256; i++)
                colorTable.push_back(qRgb(i,i,i));
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            img.setColorTable(colorTable);
            return img;
        }
        // 8-bits unsigned, NO. OF CHANNELS=3
        if(mat.type()==CV_8UC3)
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        else
        {

            return QImage();
        }
    }

    static const int LINE_METHOD = 1;
    static const int TEXTURE_METHOD = 2;
};


#endif // QTUTILS_H
