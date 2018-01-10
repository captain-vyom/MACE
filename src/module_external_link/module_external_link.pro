#-------------------------------------------------
#
# Project created by QtCreator 2017-01-13T16:56:01
#
#-------------------------------------------------
QT += serialport
QT += network
QT       -= core gui

TARGET = module_external_link
TEMPLATE = lib

DEFINES += MODULE_EXTERNAL_LINK_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += module_external_link.cpp \
    parse_comms_message.cpp \
    parse_comms_command.cpp \
    controllers/command_controller_externalLink.cpp \
    controllers/heartbeat_controller_externallink.cpp \
    controllers/home_controller_external_link.cpp \
    controllers/mission_controller.cpp

HEADERS += module_external_link.h\
        module_external_link_global.h \
    controllers/command_controller_externalLink.h \
    controllers/heartbeat_controller_externallink.h \
    controllers/home_controller_external_link.h \
    controllers/generic_controller.h \
    controller_collection.h \
    controllers/mission_controller.h \
    controllers/generic_mace_controller.h

# Unix lib Install
unix:!symbian {
    target.path = $$(MACE_ROOT)/lib
    INSTALLS += target
}

# Windows lib install
lib.path    = $$(MACE_ROOT)/lib
win32:CONFIG(release, debug|release):       lib.files   += release/module_external_link.lib release/module_external_link.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/module_external_link.lib debug/module_external_link.dll
INSTALLS += lib

#Header file copy
headers_base.path    = $$(MACE_ROOT)/include/module_external_link
headers_base.files   += \
        module_external_link_global.h \
        module_external_link.h \
        controller_collection.h
INSTALLS       += headers_base

#Header file copy
headers_controllers.path    = $$(MACE_ROOT)/include/module_external_link/controllers
headers_controllers.files   += \
    controllers/command_controller_externalLink.h \
    controllers/heartbeat_controller_externallink.h \
    controllers/home_controller_external_link.h \
    controllers/generic_controller.h \
    controllers/mission_controller.h \
    controllers/generic_mace_controller.h
INSTALLS       += headers_controllers

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../../speedLog/
INCLUDEPATH += $$PWD/../../mavlink_cpp/MACE/mace_common/
INCLUDEPATH += $$(MACE_ROOT)/Eigen/include/eigen3

INCLUDEPATH += $$(MACE_DIGIMESH_WRAPPER)/include/
LIBS += -L$$(MACE_DIGIMESH_WRAPPER)/lib/ -lMACEDigiMeshWrapper

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix:!macx: LIBS += -L$$OUT_PWD/../common/ -lcommon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data/release/ -ldata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data/debug/ -ldata
else:unix: LIBS += -L$$OUT_PWD/../data/ -ldata

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../commsMACE/release/ -lcommsMACE
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../commsMACE/debug/ -lcommsMACE
else:unix: LIBS += -L$$OUT_PWD/../commsMACE/ -lcommsMACE

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../commsMACEHelper/release/ -lcommsMACEHelper
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../commsMACEHelper/debug/ -lcommsMACEHelper
else:unix:!macx: LIBS += -L$$OUT_PWD/../commsMACEHelper/ -lcommsMACEHelper

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../mace_core/release/ -lmace_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../mace_core/debug/ -lmace_core
else:unix: LIBS += -L$$OUT_PWD/../mace_core/ -lmace_core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_item/release/ -ldata_generic_item
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_item/debug/ -ldata_generic_item
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_item/ -ldata_generic_item

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_item_topic/release/ -ldata_generic_item_topic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_item_topic/debug/ -ldata_generic_item_topic
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_item_topic/ -ldata_generic_item_topic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item/release/ -ldata_generic_state_item
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item/debug/ -ldata_generic_state_item
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_state_item/ -ldata_generic_state_item

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item_topic/release/ -ldata_generic_state_item_topic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_state_item_topic/debug/ -ldata_generic_state_item_topic
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_state_item_topic/ -ldata_generic_state_item_topic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_command_item/release/ -ldata_generic_command_item
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_command_item/debug/ -ldata_generic_command_item
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_command_item/ -ldata_generic_command_item

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_command_item_topic/release/ -ldata_generic_command_item_topic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_command_item_topic/debug/ -ldata_generic_command_item_topic
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_command_item_topic/ -ldata_generic_command_item_topic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_generic_mission_item_topic/release/ -ldata_generic_mission_item_topic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_generic_mission_item_topic/debug/ -ldata_generic_mission_item_topic
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_generic_mission_item_topic/ -ldata_generic_mission_item_topic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../data_interface_MACE/release/ -ldata_interface_MACE
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../data_interface_MACE/debug/ -ldata_interface_MACE
else:unix:!macx: LIBS += -L$$OUT_PWD/../data_interface_MACE/ -ldata_interface_MACE


