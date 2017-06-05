#ifndef GENERIC_MACE_TO_COMMS_H
#define GENERIC_MACE_TO_COMMS_H

#include <math.h>

#include "mace.h"

#include "data/coordinate_frame.h"

#include "data_generic_item/data_generic_item_components.h"
#include "data_generic_item_topic/data_generic_item_topic_components.h"

#include "data_generic_state_item/state_item_components.h"
#include "data_generic_state_item_topic/state_topic_components.h"

namespace DataCOMMS{

class Generic_MACETOCOMMS
{
public:

    mace_message_t HeartbeatTopicPTR_MACETOCOMMS(const std::shared_ptr<DataGenericItemTopic::DataGenericItemTopic_Heartbeat> &topicItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);
    mace_message_t Heartbeat_MACETOCOMMS(DataGenericItem::DataGenericItem_Heartbeat heartbeatItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);

    mace_message_t SystemArmTopicPTR_MACETOCOMMS(const std::shared_ptr<DataGenericItemTopic::DataGenericItemTopic_SystemArm> &topicItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);
    mace_message_t SystemArm_MACETOCOMMS(DataGenericItem::DataGenericItem_SystemArm systemArmItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);

    mace_message_t FlightModeTopicPTR_MACETOCOMMS(const std::shared_ptr<DataGenericItemTopic::DataGenericItemTopic_FlightMode> &topicItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);
    mace_message_t FlightMode_MACETOCOMMS(DataGenericItem::DataGenericItem_FlightMode flightModeItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);

    mace_message_t BatteryTopicPTR_MACETOCOMMS(const std::shared_ptr<DataGenericItemTopic::DataGenericItemTopic_Battery> &topicItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);
    mace_message_t Battery_MACETOCOMMS(DataGenericItem::DataGenericItem_Battery fuelItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);

    mace_message_t GPSTopicPTR_MACETOCOMMS(const std::shared_ptr<DataGenericItemTopic::DataGenericItemTopic_GPS> &topicItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);
    mace_message_t GPS_MACETOCOMMS(DataGenericItem::DataGenericItem_GPS GPSItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);

    mace_message_t TextTopicPTR_MACETOCOMMS(const std::shared_ptr<DataGenericItemTopic::DataGenericItemTopic_Text> &topicItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);
    mace_message_t Text_MACETOCOMMS(DataGenericItem::DataGenericItem_Text textItem, const int &systemID, const uint8_t &compID, const uint8_t &chan);

};

} //end of namespace DataCOMMS

#endif // GENERIC_MACE_TO_COMMS_H