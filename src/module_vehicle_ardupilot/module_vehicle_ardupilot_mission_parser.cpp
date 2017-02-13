#include "module_vehicle_ardupilot.h"

bool ModuleVehicleArdupilot::ParseMAVLINKMissionMessage(const std::string &linkName, const mavlink_message_t* message)
{
    bool parsedMissionMSG = true;
    int sysID = message->sysid;
    int compID = message->compid;
    uint8_t chan = m_LinkMarshaler->GetProtocolChannel(linkName);

    mavlink_message_t msg;

    switch ((int)message->msgid) {
    case MAVLINK_MSG_ID_MISSION_ITEM:
    {
        //This is message definition 39
        //Message encoding a mission item. This message is emitted to announce the presence of a mission item and to set a mission item on the system. The mission item can be either in x, y, z meters (type: LOCAL) or x:lat, y:lon, z:altitude. Local frame is Z-down, right handed (NED), global frame is Z-up, right handed (ENU). See also http://qgroundcontrol.org/mavlink/waypoint_protocol.
        mavlink_mission_item_t decodedMSG;
        mavlink_msg_mission_item_decode(message,&decodedMSG);
        std::shared_ptr<MissionItem::AbstractMissionItem> missionItem = DataVehicleArdupilot::ArdupilotToMACEMission::MAVLINKMissionToMACEMission(decodedMSG);

        //check to make sure the item isnt NULL
        if(missionItem)
        {
            if(decodedMSG.seq == 0)
            {

                //This is the home position item associated with the vehicle
                MissionItem::SpatialHome newHome;
                newHome.position.latitude = decodedMSG.x;
                newHome.position.longitude = decodedMSG.y;
                newHome.position.altitude = decodedMSG.z;
                std::shared_ptr<MissionTopic::MissionHomeTopic> homeTopic = std::make_shared<MissionTopic::MissionHomeTopic>();
                homeTopic->setVehicleID(sysID);
                homeTopic->setHome(newHome);

                MaceCore::TopicDatagram topicDatagram;
                m_VehicleMission.SetComponent(homeTopic, topicDatagram);

                //notify listneres of topic
                ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
                    ptr->NewTopicDataValues(m_VehicleMission.Name(), sysID, MaceCore::TIME(), topicDatagram);
                });
            }else{
                int missionIndex = decodedMSG.seq - 1;
                m_CurrentMissionQueue.at(sysID).replaceMissionItemAtIndex(missionItem,missionIndex);
            }
        }
        if(missionItemIndex == decodedMSG.seq){
            missionMSGCounter++;
            if(missionMSGCounter == 2)
            {
                missionMSGCounter = 0;
                missionItemIndex++;
                if(missionItemIndex < missionItemsAvailable)
                {
                    //This may not get handled correctly
                    mavlink_msg_mission_request_pack_chan(255,190,chan,&msg,sysID,compID,missionItemIndex);
                    m_LinkMarshaler->SendMessage<mavlink_message_t>(linkName, msg);
                }else{
                    //Reset all of the information as we have requested them all
                    missionMode = NONE;
                    missionItemIndex = 0;
                    missionItemsAvailable = 0;
                    mavlink_msg_mission_ack_pack_chan(255,190,chan,&msg,sysID,compID,MAV_MISSION_ACCEPTED);
                    m_LinkMarshaler->SendMessage<mavlink_message_t>(linkName, msg);

                    std::shared_ptr<MissionTopic::MissionListTopic> missionTopic = std::make_shared<MissionTopic::MissionListTopic>();
                    missionTopic->setVehicleID(sysID);
                    missionTopic->setMissionList(m_CurrentMissionQueue.at(sysID));

                    MaceCore::TopicDatagram topicDatagram;
                    m_VehicleMission.SetComponent(missionTopic, topicDatagram);
                    //notify listneres of topic
                    ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
                        ptr->NewTopicDataValues(m_VehicleMission.Name(), sysID, MaceCore::TIME(), topicDatagram);
                    });

                }
            }
        }
        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST:
    {
        //This is message definition 40
        //Request the information of the mission item with the sequence number seq. The response of the system to this message should be a MISSION_ITEM message. http://qgroundcontrol.org/mavlink/waypoint_protocol
        mavlink_mission_request_t decodedMSG;
        mavlink_msg_mission_request_decode(message,&decodedMSG);
        std::shared_ptr<MissionItem::AbstractMissionItem> missionItem = m_ProposedMissionQueue.at(sysID).getMissionItem(decodedMSG.seq);
        mavlink_message_t msg = MissionParserArdupilot::generateMissionMessage(missionItem,decodedMSG.seq,compID,chan);
        m_LinkMarshaler->SendMessage<mavlink_message_t>(linkName, msg);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_SET_CURRENT:
    {
        //This is message definition 41
        //Set the mission item with sequence number seq as current item. This means that the MAV will continue to this mission item on the shortest path (not following the mission items in-between).
        mavlink_mission_set_current_t decodedMSG;
        mavlink_msg_mission_set_current_decode(message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_CURRENT:
    {
        //This is message definition 42
        //Message that announces the sequence number of the current active mission item. The MAV will fly towards this mission item.
        mavlink_mission_current_t decodedMSG;
        mavlink_msg_mission_current_decode(message,&decodedMSG);

        std::shared_ptr<MissionTopic::MissionItemCurrentTopic> missionTopic = std::make_shared<MissionTopic::MissionItemCurrentTopic>();
        missionTopic->setVehicleID(sysID);
        missionTopic->setMissionItemIndex(decodedMSG.seq);

        MaceCore::TopicDatagram topicDatagram;
        m_VehicleMission.SetComponent(missionTopic, topicDatagram);
        //notify listneres of topic
        ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
            ptr->NewTopicDataValues(m_VehicleMission.Name(), sysID, MaceCore::TIME(), topicDatagram);
        });

        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
    {
        //This is message definition 43
        //Request the overall list of mission items from the system/component.
        mavlink_mission_request_list_t decodedMSG;
        mavlink_msg_mission_request_list_decode(message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_COUNT:
    {
        //This is message definition 44
        //This message is emitted as response to MISSION_REQUEST_LIST by the MAV and to initiate a write transaction. The GCS can then request the individual mission item based on the knowledge of the total number of MISSIONs.
        mavlink_mission_count_t decodedMSG;
        mavlink_msg_mission_count_decode(message,&decodedMSG);
        if(missionMode == REQUESTING)
        {
            missionItemsAvailable = decodedMSG.count;
            missionItemIndex = 0;
            m_CurrentMissionQueue.at(sysID).initializeQueue(missionItemsAvailable - 1);

            mavlink_message_t msg;
            mavlink_msg_mission_request_pack_chan(255,190,chan,&msg,sysID,compID,missionItemIndex);
            m_LinkMarshaler->SendMessage<mavlink_message_t>(linkName, msg);
        }
        break;
    }
    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
    {
        //This is message definition 45
        //Delete all mission items at once.
        mavlink_mission_clear_all_t decodedMSG;
        mavlink_msg_mission_clear_all_decode(message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_ITEM_REACHED:
    {
        //This is message definition 46
        //A certain mission item has been reached. The system will either hold this position (or circle on the orbit) or (if the autocontinue on the WP was set) continue to the next MISSION.
        mavlink_mission_item_reached_t decodedMSG;
        mavlink_msg_mission_item_reached_decode(message,&decodedMSG);

        std::shared_ptr<MissionTopic::MissionItemReachedTopic> missionTopic = std::make_shared<MissionTopic::MissionItemReachedTopic>();
        missionTopic->setVehicleID(sysID);
        missionTopic->setMissionItemIndex(decodedMSG.seq);

        MaceCore::TopicDatagram topicDatagram;
        m_VehicleMission.SetComponent(missionTopic, topicDatagram);
        //notify listneres of topic
        ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
            ptr->NewTopicDataValues(m_VehicleMission.Name(), sysID, MaceCore::TIME(), topicDatagram);
        });
        break;
    }
    case MAVLINK_MSG_ID_MISSION_ACK:
    {
        //This is message definition 47
        //Ack message during MISSION handling. The type field states if this message is a positive ack (type=0) or if an error happened (type=non-zero).
        mavlink_mission_ack_t decodedMSG;
        mavlink_msg_mission_ack_decode(message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION:
    {
        //This is message definition 242
        //This message can be requested by sending the MAV_CMD_GET_HOME_POSITION command. The position the system will return to and land on. The position is set automatically by the system during the takeoff in case it was not explicitely set by the operator before or after. The position the system will return to and land on. The global and local positions encode the position in the respective coordinate frames, while the q parameter encodes the orientation of the surface. Under normal conditions it describes the heading and terrain slope, which can be used by the aircraft to adjust the approach. The approach 3D vector describes the point to which the system should fly in normal flight mode and then perform a landing sequence along the vector.
        mavlink_home_position_t decodedMSG;
        mavlink_msg_home_position_decode(message,&decodedMSG);

        MissionItem::SpatialHome spatialHome;
        spatialHome.position.latitude = decodedMSG.latitude;
        spatialHome.position.longitude = decodedMSG.longitude;
        spatialHome.position.altitude = decodedMSG.altitude;
        spatialHome.setVehicleID(sysID);

        std::shared_ptr<MissionTopic::MissionHomeTopic> missionTopic = std::make_shared<MissionTopic::MissionHomeTopic>();
        missionTopic->setVehicleID(sysID);
        missionTopic->setHome(spatialHome);

        MaceCore::TopicDatagram topicDatagram;
        m_VehicleMission.SetComponent(missionTopic, topicDatagram);
        //notify listneres of topic
        ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
            ptr->NewTopicDataValues(m_VehicleMission.Name(), sysID, MaceCore::TIME(), topicDatagram);
        });

        break;
    }

    default:
    {
        parsedMissionMSG = false;
    }
    }
    return parsedMissionMSG;
}

void ModuleVehicleArdupilot::MissionAcknowledgement(const MAV_MISSION_RESULT &missionResult, const bool &publishResult)
{
    switch(missionResult) {
    case MAV_MISSION_ACCEPTED:
    {
        break;
    }
    case MAV_MISSION_ERROR:
    {
        break;
    }
    case MAV_MISSION_UNSUPPORTED_FRAME:
    {
        break;
    }
    case MAV_MISSION_UNSUPPORTED:
    {
        break;
    }
    case MAV_MISSION_NO_SPACE:
    {
        break;
    }
    case MAV_MISSION_INVALID:
    case MAV_MISSION_INVALID_PARAM1:
    case MAV_MISSION_INVALID_PARAM2:
    case MAV_MISSION_INVALID_PARAM3:
    case MAV_MISSION_INVALID_PARAM4:
    case MAV_MISSION_INVALID_PARAM5_X:
    case MAV_MISSION_INVALID_PARAM6_Y:
    case MAV_MISSION_INVALID_PARAM7:
    {
        std::cout<<"One of the parameters has an invalid value"<<std::endl;
        break;
    }
    case MAV_MISSION_DENIED:
    {
        break;
    }
    default:
    {
        break;
    }
    }
}
