#include <module_PalmImageReader/SinglePalmReader.h>
#include <utility/PPAException.h>
#include <QString>
#include <QStringList>
#include <QDebug>
using namespace std;

SinglePalmReader::SinglePalmReader()
{
}

void SinglePalmReader::init(std::string path) {
	this->path = path;
}

cv::Mat SinglePalmReader::readPalmImage() {
	
    cv::Mat input = cv::imread(path);
	
	if (!input.data) throw PPAException("Error during reading image");

	return input;
}

int SinglePalmReader::readUserId(){

    // First three digit of the filename represents the user id
    QString qpath = QString::fromStdString(path);
    QStringList tokens = qpath.split("/");
    QString imagename = tokens.at(tokens.size()-1);

    return imagename.left(3).toInt();
}


SinglePalmReader::~SinglePalmReader()
{
}
