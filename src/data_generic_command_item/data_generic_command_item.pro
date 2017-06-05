#-------------------------------------------------
#
# Project created by QtCreator 2017-05-17T16:35:57
#
#-------------------------------------------------


QT       -= core gui

TARGET = data_generic_command_item
TEMPLATE = lib

DEFINES += DATA_GENERIC_COMMAND_ITEM_LIBRARY

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
    do_items/action_arm.cpp \
    do_items/action_change_mode.cpp \
    do_items/action_change_speed.cpp \
    do_items/action_motor_test.cpp \
    do_items/action_mission_command.cpp \
    spatial_items/spatial_home.cpp \
    spatial_items/spatial_land.cpp \
    spatial_items/spatial_loiter_time.cpp \
    spatial_items/spatial_loiter_turns.cpp \
    spatial_items/spatial_loiter_unlimited.cpp \
    spatial_items/spatial_rtl.cpp \
    spatial_items/spatial_takeoff.cpp \
    spatial_items/spatial_waypoint.cpp \
    mission_items/mission_ack.cpp \
    mission_items/mission_list.cpp \
    command_item_ack.cpp
HEADERS +=\
    do_items/action_arm.h \
    do_items/action_change_mode.h \
    do_items/action_change_speed.h \
    do_items/action_motor_test.h\
    do_items/do_components.h \
    do_items/action_mission_command.h \
    mission_items/mission_ack.h \
    mission_items/mission_list.h \
    spatial_items/spatial_components.h \
    spatial_items/spatial_home.h \
    spatial_items/spatial_land.h \
    spatial_items/spatial_loiter_time.h \
    spatial_items/spatial_loiter_turns.h \
    spatial_items/spatial_loiter_unlimited.h \
    spatial_items/spatial_rtl.h \
    spatial_items/spatial_takeoff.h \
    spatial_items/spatial_waypoint.h \
    data_generic_command_item_global.h \
    command_item_components.h \
    abstract_command_item.h \
    command_item_ack.h

# Unix lib Install
unix:!symbian {
    target.path = $$(MACE_ROOT)/lib
    INSTALLS += target
}

# Windows lib install
lib.path    = $$(MACE_ROOT)/lib
win32:CONFIG(release, debug|release):       lib.files   += release/data_generic_command_item.lib release/data_generic_command_item.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/data_generic_command_item.lib debug/data_generic_command_item.dll
INSTALLS += lib

#Header file copy
headers.path    = $$(MACE_ROOT)/include/data_generic_command_item
headers.files   += \
    data_generic_command_item_global.h \
    abstract_command_item.h \
    command_item_ack.h \
    command_item_components.h
INSTALLS       += headers

#Header file copy
headers_doComponents.path    = $$(MACE_ROOT)/include/data_generic_command_item/do_items
headers_doComponents.files   += \
    do_items/action_arm.h \
    do_items/action_change_mode.h \
    do_items/action_change_speed.h \
    do_items/action_motor_test.h \
    do_items/action_mission_command.h \
    do_items/do_components.h
INSTALLS       += headers_doComponents

#Header file copy
headers_missionComponents.path    = $$(MACE_ROOT)/include/data_generic_command_item/mission_items
headers_missionComponents.files   += \
    mission_items/mission_ack.h \
    mission_items/mission_list.h
INSTALLS       += headers_missionComponents

#Header file copy
headers_spatialComponents.path    = $$(MACE_ROOT)/include/data_generic_command_item/spatial_items
headers_spatialComponents.files   += \
    spatial_items/spatial_components.h \
    spatial_items/spatial_home.h \
    spatial_items/spatial_land.h \
    spatial_items/spatial_loiter_time.h \
    spatial_items/spatial_loiter_turns.h \
    spatial_items/spatial_loiter_unlimited.h \
    spatial_items/spatial_rtl.h \
    spatial_items/spatial_takeoff.h \
    spatial_items/spatial_waypoint.h
INSTALLS       += headers_spatialComponents

INCLUDEPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data/release/ -ldata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data/debug/ -ldata
else:unix: LIBS += -L$$OUT_PWD/../data/ -ldata

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item/release/ -ldata_generic_state_item
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item/debug/ -ldata_generic_state_item
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_state_item/ -ldata_generic_state_item

INCLUDEPATH += $$(MACE_ROOT)/Eigen/include/eigen3
