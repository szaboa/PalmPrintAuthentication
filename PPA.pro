#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T13:20:55
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PPA
TEMPLATE = app

DESTDIR=bin #Target file directory
OBJECTS_DIR=generated_files #Intermediate object files directory
MOC_DIR=generated_files #Intermediate moc files directory

SOURCES += main.cpp\
    utility/BoundaryTracking.cpp \
    utility/Thinning.cpp \
    EnrollmentThread.cpp \
    json/json11.cpp \
    module_FeatureExtraction/PrincipalLineFeature.cpp \
    module_FeatureExtraction/TextureFeature.cpp \
    module_DatabaseAdapter/DbAdapter.cpp \
    module_Matcher/PrincipalLineMatcher.cpp \
    module_Matcher/TextureMatcher.cpp \
    AuthenticationThread.cpp \
    SingleVerificationThread.cpp \
    MainWindow.cpp \
    module_Preprocessing/Preprocessor.cpp \
    module_RoiExtraction/SquareRoiExtractor.cpp \
    module_FeatureExtraction/PrincipalLineExtractor.cpp \
    module_FeatureExtraction/TextureExtractor.cpp \
    module_PalmImageReader/MultPalmReader.cpp \
    module_PalmImageReader/SinglePalmReader.cpp

HEADERS  += \
    module_PalmImageReader/IPalmReader.h \
    utility/BoundaryTracking.h \
    utility/PPAException.h \
    utility/Thinning.h \
    utility/QtUtils.h \
    EnrollmentThread.h \
    ui_mainwindow.h \
    json/json11.hpp \
    ui_mainwindow.h \
    module_FeatureExtraction/IFeature.h \
    module_FeatureExtraction/PrincipalLineFeature.h \
    module_FeatureExtraction/TextureFeature.h \
    module_DatabaseAdapter/DbAdapter.h \
    module_DatabaseAdapter/JsonPoint.h \
    easylogging/easylogging++.h \
    module_Matcher/IMatcher.h \
    module_Matcher/PrincipalLineMatcher.h \
    module_Matcher/TextureMatcher.h \
    sqlite/hdr/sqlite_modern_cpp/extensions/boost_json_spirit.h \
    sqlite/hdr/sqlite_modern_cpp/extensions/boost_uuid.h \
    sqlite/hdr/sqlite_modern_cpp/extensions/std_time_t.h \
    sqlite/hdr/sqlite_modern_cpp/utility/function_traits.h \
    sqlite/hdr/sqlite_modern_cpp.h \
    sqlite/sqlite3.h \
    AuthenticationThread.h \
    SingleVerificationThread.h \
    MainWindow.h \
    module_Preprocessing/IPreprocessor.h \
    module_Preprocessing/Preprocessor.h \
    module_RoiExtraction/IRoiExtractor.h \
    module_RoiExtraction/SquareRoiExtractor.h \
    module_FeatureExtraction/IFeatureExtractor.h \
    module_FeatureExtraction/PrincipalLineExtractor.h \
    module_FeatureExtraction/TextureExtractor.h \
    module_PalmImageReader/MultPalmReader.h \
    module_PalmImageReader/SinglePalmReader.h

FORMS    += mainwindow.ui

INCLUDEPATH += "/user/include/"
INCLUDEPATH += ".sqlite/"
INCLUDEPATH += "./sqlite/hdr/"
INCLUDEPATH += "./easylogging/"

LIBS  += `pkg-config --libs opencv`

LIBS += -lboost_system \
        -lboost_filesystem

LIBS += -lsqlite3

CONFIG += release
QMAKE_CXXFLAGS_RELEASE += -O3

