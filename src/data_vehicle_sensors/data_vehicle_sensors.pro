#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T09:07:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = data_vehicle_sensors
TEMPLATE = lib

DEFINES += DATA_VEHICLE_SENSORS_LIBRARY

QMAKE_CXXFLAGS += -std=c++11


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/sensor_camera.cpp

HEADERS += data_vehicle_sensors_global.h \
    components.h \
    components/sensor_camera.h

# Unix lib Install
unix:!symbian {
    target.path = $$(MACE_ROOT)/lib
    INSTALLS += target
}

# Windows lib install
lib.path    = $$(MACE_ROOT)/lib
win32:CONFIG(release, debug|release):       lib.files   += release/data_vehicle_sensors.lib release/data_vehicle_sensors.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/data_vehicle_sensors.lib debug/data_vehicle_sensors.dll
INSTALLS += lib

#Header file copy
headers.path    = $$(MACE_ROOT)/include/data_vehicle_sensors
headers.files   += \
        data_vehicle_sensors_global.h \
        components.h
INSTALLS       += headers

#Header file copy
headers_Components.path    = $$(MACE_ROOT)/include/data_vehicle_sensors/Components
headers_Components.files   += \
        components/sensor_camera.h
INSTALLS       += headers_Components


INCLUDEPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../mace_core/release/ -lmace_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../mace_core/debug/ -lmace_core
else:unix: LIBS += -L$$OUT_PWD/../mace_core/ -lmace_core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data/release/ -ldata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data/debug/ -ldata
else:unix: LIBS += -L$$OUT_PWD/../data/ -ldata