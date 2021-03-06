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
    command_item_ack.cpp \
    mission_items/mission_item_achieved.cpp \
    mission_items/mission_item_current.cpp \
    mission_items/mission_key.cpp \
    mission_items/mission_key_change.cpp \
    boundary_items/boundary_key.cpp \
    boundary_items/boundary_list.cpp \
    target_items/dynamic_target_list.cpp \
    target_items/dynamic_mission_queue.cpp \
    target_items/dynamic_target.cpp \
    target_items/dynamic_target_storage.cpp
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
    command_item_ack.h \
    mission_items/mission_item_achieved.h \
    mission_items/mission_item_current.h \
    mission_items/mission_state.h \
    mission_items/mission_type.h \
    mission_items/mission_key.h \
    mission_items/mission_key_change.h \
    spatial_items/abstract_spatial_position.h \
    command_item_type.h \
    boundary_items/boundary_key.h \
    boundary_items/boundary_type.h \
    boundary_items/boundary_list.h \
    target_items/dynamic_target_list.h \
    target_items/dynamic_mission_queue.h \
    target_items/dynamic_target.h \
    target_items/dynamic_target_storage.h

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
INSTALL_PREFIX = $$(MACE_ROOT)/include/$$TARGET
INSTALL_HEADERS = $$HEADERS
include(../headerinstall.pri)


INCLUDEPATH += $$PWD/../../mavlink_cpp/MACE/mace_common/
INCLUDEPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix:!macx: LIBS += -L$$OUT_PWD/../common/ -lcommon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../base/release/ -lbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../base/debug/ -lbase
else:unix:!macx: LIBS += -L$$OUT_PWD/../base/ -lbase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data/release/ -ldata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data/debug/ -ldata
else:unix: LIBS += -L$$OUT_PWD/../data/ -ldata

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item/release/ -ldata_generic_state_item
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item/debug/ -ldata_generic_state_item
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_state_item/ -ldata_generic_state_item

INCLUDEPATH += $$(MACE_ROOT)/Eigen/include/eigen3

