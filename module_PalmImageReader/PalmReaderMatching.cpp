#include <module_PalmImageReader/PalmReaderMatching.h>
#include <utility/PPAException.h>
#include <QString>
#include <QStringList>
#include <QDebug>
using namespace std;

PalmReaderMatching::PalmReaderMatching()
{
}

void PalmReaderMatching::init(std::string path) {
	this->path = path;
}

cv::Mat PalmReaderMatching::readPalmImage() {
	
    cv::Mat input = cv::imread(path);
	
	if (!input.data) throw PPAException("Error during reading image");

	return input;
}

int PalmReaderMatching::readUserId(){
    QString qpath = QString::fromStdString(path);
    QStringList tokens = qpath.split("/");

    QString imagename = tokens.at(tokens.size()-1);


    return imagename.left(3).toInt();
}


PalmReaderMatching::~PalmReaderMatching()
{
}
