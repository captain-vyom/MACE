#-------------------------------------------------
#
# Project created by QtCreator 2017-01-12T13:32:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = data_vehicle_ardupilot
TEMPLATE = lib

DEFINES += DATA_VEHICLE_ARDUPILOT_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

SOURCES +=

HEADERS +=\
        data_vehicle_ardupilot_global.h \
    mavlink_parser_ardupilot.h

# Windows lib install
lib.path    = $$(MACE_ROOT)/lib
win32:CONFIG(release, debug|release):       lib.files   += release/module_vehicle_MAVLINK.lib release/module_vehicle_MAVLINK.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/module_vehicle_MAVLINK.lib debug/module_vehicle_MAVLINK.dll
INSTALLS += lib

#Header file copy
headers.path    = $$(MACE_ROOT)/include/module_vehicle_MAVLINK
headers.files   += \
        module_vehicle_mavlink.h \
        module_vehicle_mavlink_global.h \
        generic_message_definition_mavlink.h
INSTALLS       += headers



INCLUDEPATH += $$PWD/../../mavlink_cpp/V2/ardupilotmega

INCLUDEPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../mace_core/release/ -lmace_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../mace_core/debug/ -lmace_core
else:unix: LIBS += -L$$OUT_PWD/../mace_core/ -lmace_core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_vehicle_generic/release/ -ldata_vehicle_generic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_vehicle_generic/debug/ -ldata_vehicle_generic
else:unix: LIBS += -L$$OUT_PWD/../data_vehicle_generic/ -ldata_vehicle_generic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_vehicle_MAVLINK/release/ -ldata_vehicle_MAVLINK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_vehicle_MAVLINK/debug/ -ldata_vehicle_MAVLINK
else:unix: LIBS += -L$$OUT_PWD/../data_vehicle_MAVLINK/ -ldata_vehicle_MAVLINK
