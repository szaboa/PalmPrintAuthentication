#ifndef QTUTILS_H
#define QTUTILS_H

#include <QString>
#include <QMessageBox>
#include <sstream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;
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

    static string serialize(Mat input)
    {
        // We will need to also serialize the width, height, type and size of the matrix
        int width = input.cols;
        int height = input.rows;
        int type = input.type();
        size_t size = input.total() * input.elemSize();

        // Initialize a stringstream and write the data
        stringstream ss;
        ss.write((char*)(&width), sizeof(int));
        ss.write((char*)(&height), sizeof(int));
        ss.write((char*)(&type), sizeof(int));
        ss.write((char*)(&size), sizeof(size_t));

        // Write the whole image data
        ss.write((char*)input.data, size);

        return ss.str();
    }

    // Deserialize a Mat from a stringstream
    static Mat deserialize(stringstream& input)
    {
        // The data we need to deserialize
        int width = 0;
        int height = 0;
        int type = 0;
        size_t size = 0;

        // Read the width, height, type and size of the buffer
        input.read((char*)(&width), sizeof(int));
        input.read((char*)(&height), sizeof(int));
        input.read((char*)(&type), sizeof(int));
        input.read((char*)(&size), sizeof(size_t));

        // Allocate a buffer for the pixels
        char* data = new char[size];
        // Read the pixels from the stringstream
        input.read(data, size);

        // Construct the image (clone it so that it won't need our buffer anymore)
        Mat m = Mat(height, width, type, data).clone();

        // Delete our buffer
        delete[]data;

        // Return the matrix
        return m;
    }

    static const int LINE_METHOD = 1;
    static const int TEXTURE_METHOD = 2;
};


#endif // QTUTILS_H
