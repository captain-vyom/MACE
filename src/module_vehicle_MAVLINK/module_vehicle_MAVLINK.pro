#-------------------------------------------------
#
# Project created by QtCreator 2016-08-24T11:14:11
#
#-------------------------------------------------

QT += serialport
QT += network
QT       -= core gui

TARGET = module_vehicle_MAVLINK
TEMPLATE = lib

DEFINES += MODULE_VEHICLE_MAVLINK_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

SOURCES += module_vehicle_mavlink.cpp

HEADERS += module_vehicle_mavlink.h\
        module_vehicle_mavlink_global.h


# Unix lib Install
unix:!symbian {
    target.path = $$(MACE_ROOT)/lib
    INSTALLS += target
}

# Windows lib install
lib.path    = $$(MACE_ROOT)/lib
win32:CONFIG(release, debug|release):       lib.files   += release/module_vehicle_MAVLINK.lib release/module_vehicle_MAVLINK.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/module_vehicle_MAVLINK.lib debug/module_vehicle_MAVLINK.dll
INSTALLS += lib

#Header file copy
headers.path    = $$(MACE_ROOT)/include/module_vehicle_MAVLINK
headers.files   += $$HEADERS
INSTALLS       += headers

INCLUDEPATH += $$(MACE_ROOT)/include


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../mace_core/release/ -lmace_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../mace_core/debug/ -lmace_core
else:unix: LIBS += -L$$OUT_PWD/../mace_core/ -lmace_core


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../comms/release/ -lcomms
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../comms/debug/ -lcomms
else:unix: LIBS += -L$$OUT_PWD/../comms/ -lcomms


INCLUDEPATH += $$PWD/../../mavlink_cpp/V2/ardupilotmega


unix{
    EigenInclude = $$system(pkg-config --cflags eigen3)
    EigenInclude = $$replace(EigenInclude, "-I", "")/eigen3
    INCLUDEPATH += $$EigenInclude
}
win32{
    INCLUDEPATH += "C:\Program Files (x86)\Eigen\include\eigen3"
}


INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

