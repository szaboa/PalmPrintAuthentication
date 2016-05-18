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
        mainwindow.cpp \
    module_PalmImageReader/PalmReaderEnrollment.cpp \
    module_PalmImageReader/PalmReaderMatching.cpp \
    module_Preprocessing/Preprocessing.cpp \
    utility/BoundaryTracking.cpp \
    utility/Histogram.cpp \
    utility/MorphologicalOperation.cpp \
    utility/Thinning.cpp \
    EnrollmentThread.cpp \
    module_RoiExtraction/RegionGrowing.cpp \
    module_RoiExtraction/SquareRoiExtraction.cpp \
    json/json11.cpp \
    module_FeatureExtraction/PrincipalLineExtraction.cpp \
    module_FeatureExtraction/PrincipalLineFeature.cpp \
    module_FeatureExtraction/TextureExtraction.cpp \
    module_FeatureExtraction/TextureFeature.cpp \
    module_DatabaseAdapter/DbAdapter.cpp \
    module_RoiExtraction/SkinModelSegmentation.cpp \
    module_Matcher/PrincipalLineMatcher.cpp \
    module_Matcher/TextureMatcher.cpp \
    AuthenticationThread.cpp

HEADERS  += mainwindow.h \
    module_PalmImageReader/IPalmReader.h \
    module_PalmImageReader/PalmReaderEnrollment.h \
    module_PalmImageReader/PalmReaderMatching.h \
    module_Preprocessing/IPreprocessing.h \
    module_Preprocessing/Preprocessing.h \
    utility/BoundaryTracking.h \
    utility/Histogram.h \
    utility/MorphologicalOperation.h \
    utility/PPAException.h \
    utility/Thinning.h \
    utility/QtUtils.h \
    EnrollmentThread.h \
    module_RoiExtraction/IRegionSegmentation.h \
    module_RoiExtraction/IRoiExtraction.h \
    module_RoiExtraction/RegionGrowing.h \
    module_RoiExtraction/SquareRoiExtraction.h \
    ui_mainwindow.h \
    json/json11.hpp \
    ui_mainwindow.h \
    module_FeatureExtraction/IFeature.h \
    module_FeatureExtraction/IFeatureExtraction.h \
    module_FeatureExtraction/PrincipalLineExtraction.h \
    module_FeatureExtraction/PrincipalLineFeature.h \
    module_FeatureExtraction/TextureExtraction.h \
    module_FeatureExtraction/TextureFeature.h \
    module_DatabaseAdapter/DbAdapter.h \
    module_DatabaseAdapter/JsonPoint.h \
    module_RoiExtraction/SkinModelSegmentation.h \
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
    AuthenticationThread.h

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

