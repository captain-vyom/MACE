#include "module_vehicle_ardupilot.h"

ModuleVehicleArdupilot::ModuleVehicleArdupilot() :
    ModuleVehicleMAVLINK<DATA_VEHICLE_ARDUPILOT_TYPES>(),
    m_VehicleMission("vehicleMission"), m_AircraftController(NULL)
{

}

std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> ModuleVehicleArdupilot::getArducopterData(const int &systemID)
{
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData;
    try{
        tmpData = m_ArduPilotData.at(systemID);
    }catch(const std::out_of_range &oor)
    {
        tmpData = std::make_shared<DataARDUPILOT::VehicleObject_ARDUPILOT>(systemID,255,0);
        m_ArduPilotData.insert({systemID,tmpData});

        ModuleVehicleMavlinkBase::NotifyListeners([&](MaceCore::IModuleEventsVehicle* ptr){
            ptr->NewConstructedVehicle(this, systemID);
        });

//        std::shared_ptr<Ardupilot_GuidedController> newController = std::make_shared<Ardupilot_GuidedController>(tmpData, m_LinkMarshaler, m_LinkName, m_LinkChan);
//        m_ArdupilotController[systemID] = newController;

//        std::thread *thread = new std::thread([newController]()
//        {
//            newController->start();
//        });
    }

    return tmpData;
}

//!
//! \brief This module as been attached as a module
//! \param ptr pointer to object that attached this instance to itself
//!
void ModuleVehicleArdupilot::AttachedAsModule(MaceCore::IModuleTopicEvents* ptr)
{
    ptr->Subscribe(this, m_VehicleMission.Name());
}

////////////////////////////////////////////////////////////////////////////
/// GENERAL VEHICLE COMMAND EVENTS: These are events that may have a direct
/// command and action sequence that accompanies the vheicle. Expect an
/// acknowledgement or an event to take place when calling these items.
////////////////////////////////////////////////////////////////////////////

void ModuleVehicleArdupilot::Command_ChangeVehicleArm(const MissionItem::ActionArm &vehicleArm)
{
    int vehicleID = vehicleArm.getVehicleID();
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
    mavlink_message_t msg = tmpData->generateArmMessage(vehicleArm,m_LinkChan);
    m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
}

void ModuleVehicleArdupilot::Command_ChangeVehicleOperationalMode(const MissionItem::ActionChangeMode &vehicleMode)
{
    int vehicleID = vehicleMode.getVehicleID();
    std::string modeString = vehicleMode.getRequestMode();

    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
    if(tmpData->data->getHearbeatSeen())
    {
        int newFlightMode = tmpData->data->ArdupilotFlightMode.get().getFlightModeFromString(modeString);
        mavlink_message_t msg = tmpData->generateChangeMode(vehicleID,m_LinkChan,newFlightMode);
        m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
    }
}

void ModuleVehicleArdupilot::SpinUpController(Ardupilot_GeneralController *newController) {

    m_AircraftController = newController;
    m_AircraftController->start();
}

void ModuleVehicleArdupilot::SpinDownController() {
    m_AircraftController->terminateObject();
}

void ModuleVehicleArdupilot::Command_RequestVehicleTakeoff(const MissionItem::SpatialTakeoff<DataState::StateGlobalPosition> &vehicleTakeoff)
{
    int vehicleID = vehicleTakeoff.getVehicleID();
//    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
//    mavlink_message_t msg = tmpData->generateTakeoffMessage(vehicleTakeoff,m_LinkChan);
//    m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
    Ardupilot_TakeoffController* newController = new Ardupilot_TakeoffController(tmpData, m_LinkMarshaler, m_LinkName, m_LinkChan);
    if(vehicleTakeoff.getPositionFlag())
        newController->initializeTakeoffSequence(vehicleTakeoff);
    else{
        MissionItem::SpatialTakeoff<DataState::StateGlobalPosition> defaultTakeoff = vehicleTakeoff;
        defaultTakeoff.position.altitude = 20;
        newController->initializeTakeoffSequence(defaultTakeoff);
    }

    this->SpinUpController(newController);
}

/////////////////////////////////////////////////////////////////////////////
/// GENERAL HOME EVENTS: These events are related to establishing or setting
/// a home position. It should be recognized that the first mission item in a
/// mission queue should prepend this position. Just the way ardupilot works.
/////////////////////////////////////////////////////////////////////////////

void ModuleVehicleArdupilot::Command_GetHomePosition(const int &vehicleID)
{
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
    mavlink_message_t msg = tmpData->generateGetHomeMessage(vehicleID,m_LinkChan);
    m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
}

void ModuleVehicleArdupilot::Command_SetHomePosition(const MissionItem::SpatialHome &vehicleHome)
{
    int vehicleID = vehicleHome.getVehicleID();
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
    mavlink_message_t msg = tmpData->generateSetHomePosition(vehicleHome,m_LinkChan);
    m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
}

void ModuleVehicleArdupilot::homePositionUpdated(const MissionItem::SpatialHome &newVehicleHome)
{
    int systemID = newVehicleHome.getVehicleID();
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(systemID);

    std::shared_ptr<MissionTopic::MissionHomeTopic> homeTopic = std::make_shared<MissionTopic::MissionHomeTopic>();
    homeTopic->setHome(newVehicleHome);

    if(tmpData->data->m_MissionHome == NULL || *homeTopic != *tmpData->data->m_MissionHome)
    {
        tmpData->data->m_MissionHome = homeTopic;
    }

    MaceCore::TopicDatagram topicDatagram;
    m_VehicleMission.SetComponent(homeTopic, topicDatagram);

    //notify listneres of topic
    ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
        ptr->NewTopicDataValues(this, m_VehicleMission.Name(), systemID, MaceCore::TIME(), topicDatagram);
    });

    //notify the core of the change
    ModuleVehicleMavlinkBase::NotifyListeners([&](MaceCore::IModuleEventsVehicle* ptr){
        ptr->NewVehicleHomePosition(this, newVehicleHome);
    });
}

/////////////////////////////////////////////////////////////////////////
/// GENERAL MISSION EVENTS: This is implying for auto mode of the vehicle.
/// This functionality may be pertinent for vehicles not containing a
/// direct MACE hardware module.
/////////////////////////////////////////////////////////////////////////

void ModuleVehicleArdupilot::UpdateMissionKey(const Data::MissionKeyChange &key)
{
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(key.oldKey.m_systemID);
    MissionItem::MissionList missionList = tmpData->data->Command_GetCurrentMission(key.oldKey.m_missionType);
    if(missionList.getMissionKey() == key.oldKey)
    {
        missionList.setMissionKey(key.newKey);
        tmpData->data->Command_SetCurrentMission(missionList);
    }
}

void ModuleVehicleArdupilot::Command_UploadMission(const MissionItem::MissionList &missionList)
{
    switch(missionList.getMissionType())
    {
    case(Data::MissionType::AUTO): //This case should push the mission directly to the aircraft
    {
        int vehicleID = missionList.getVehicleID();
        std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(vehicleID);
        tmpData->data->setProposedMission(missionList);
        mavlink_message_t msg;
        int queueSize = missionList.getQueueSize();

        mavlink_msg_mission_count_pack_chan(255,190,m_LinkChan,&msg,vehicleID,0,queueSize,MAV_MISSION_TYPE_MISSION);
        m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
        break;
    }
    case(Data::MissionType::GUIDED):
    {
        //In these two cases the mission should be carefully considered if we are a module
        //aboard MACE hardware companion package or communicating via ground link
        if(airborneInstance)
        {
            //If we are an airborne instance we can acknowledge this right away
            //This implies we are aboard the aircraft directly communicating with the autopilot
            //Thus higher rate capabilities and request for state are available
        }else{

        }
        break;
    }
    default:
        break;
    }
}


void ModuleVehicleArdupilot::Command_SetCurrentMission(const Data::MissionKey &key)
{
    UNUSED(key);
}

void ModuleVehicleArdupilot::Command_GetCurrentMission(const int &targetSystem)
{
    UNUSED(targetSystem);
}

void ModuleVehicleArdupilot::Command_GetMission(const Data::MissionKey &key)
{
    UNUSED(key);
}

void ModuleVehicleArdupilot::Command_ClearCurrentMission(const int &targetSystem)
{
    UNUSED(targetSystem);
}

////////////////////////////////////////////////////////////////////////////
/// GENERAL AUTO EVENTS: This is implying for auto mode of the vehicle.
/// This functionality is pertinent for vehicles that may contain a
/// MACE HW module, or, vehicles that have timely or ever updating changes.
////////////////////////////////////////////////////////////////////////////

void ModuleVehicleArdupilot::Command_GetOnboardAuto(const int &targetSystem)
{
    mavlink_message_t msg;
    mavlink_msg_mission_request_list_pack_chan(255,190,m_LinkChan,&msg,targetSystem,0,MAV_MISSION_TYPE_MISSION);
    m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
}

void ModuleVehicleArdupilot::Command_ClearOnboardAuto(const int &targetSystem)
{
    mavlink_message_t msg;
    mavlink_msg_mission_clear_all_pack_chan(255,190,m_LinkChan,&msg,targetSystem,0,MAV_MISSION_TYPE_MISSION);
    m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);
}

/////////////////////////////////////////////////////////////////////////
/// GENERAL GUIDED EVENTS: This is implying for guided mode of the vehicle.
/// This functionality is pertinent for vehicles that may contain a
/// MACE HW module, or, vehicles that have timely or ever updating changes.
/////////////////////////////////////////////////////////////////////////

void ModuleVehicleArdupilot::Command_GetOnboardGuided(const int &targetSystem)
{
    UNUSED(targetSystem);
}

void ModuleVehicleArdupilot::Command_ClearOnboardGuided(const int &targetSystem)
{
    UNUSED(targetSystem);
}


void ModuleVehicleArdupilot::VehicleHeartbeatInfo(const std::string &linkName, const int systemID, const mavlink_heartbeat_t &heartbeatMSG)
{
    UNUSED(linkName);
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(systemID);
    //The purpose of this module seeing if it is the first heartbeat is to establish initial comms and parameter grabs
    //from the vehicle.
    if(!tmpData->data->getHearbeatSeen())
    {
        tmpData->data->setHeartbeatSeen(true);
        mavlink_message_t msg;
        mavlink_msg_mission_request_list_pack_chan(255,190,m_LinkChan,&msg,systemID,0,MAV_MISSION_TYPE_MISSION);
        m_LinkMarshaler->SendMessage<mavlink_message_t>(m_LinkName, msg);

    }
    DataARDUPILOT::VehicleFlightMode newDataMode;
    newDataMode.parseMAVLINK(heartbeatMSG);

    tmpData->data->ArdupilotFlightMode.set(newDataMode);

    std::shared_ptr<DataARDUPILOT::VehicleFlightMode> ptrMode = std::make_shared<DataARDUPILOT::VehicleFlightMode>(newDataMode);

    MaceCore::TopicDatagram topicDatagram;
    m_VehicleDataTopic.SetComponent(ptrMode, topicDatagram);
    //notify listneres of topic
    ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
        ptr->NewTopicDataValues(this, m_VehicleDataTopic.Name(), systemID, MaceCore::TIME(), topicDatagram);
    });
}

void ModuleVehicleArdupilot::VehicleCommandACK(const std::string &linkName, const int systemID, const mavlink_command_ack_t &cmdACK)
{
    UNUSED(linkName);
    UNUSED(systemID);
    if(checkControllerState())
        m_AircraftController->updateCommandACK(cmdACK);
}

//!
//! \brief New Mavlink message received over a link
//! \param linkName Name of link message received over
//! \param msg Message received
//!
void ModuleVehicleArdupilot::MavlinkMessage(const std::string &linkName, const mavlink_message_t &message)
{
    int systemID = message.sysid;
    std::shared_ptr<DataARDUPILOT::VehicleObject_ARDUPILOT> tmpData = getArducopterData(systemID);

    bool wasMissionMSG = ParseMAVLINKMissionMessage(tmpData, linkName, &message);

    if(wasMissionMSG == false){
        //generate topic datagram from given mavlink message
        std::vector<std::shared_ptr<Data::ITopicComponentDataObject>> components = tmpData->parser->ParseForVehicleData(&message);
        //proceed to send components only if there is 1 or more
        if(components.size() > 0)
        {
            //construct datagram
            MaceCore::TopicDatagram topicDatagram;
            for(size_t i = 0 ; i < components.size() ; i++)
            {
                m_VehicleDataTopic.SetComponent(components.at(i), topicDatagram);

                /*
                 * No longer needed. Controller automatically updated when approiate m_ArduPilotData object updated
                 *
                if(components.at(i)->name == DataStateTopic::StateAttitudeTopic::Name()) {
                    std::shared_ptr<DataStateTopic::StateAttitudeTopic> component = std::make_shared<DataStateTopic::StateAttitudeTopic>();
                    m_VehicleDataTopic.GetComponent(component, topicDatagram);
                    if(checkControllerState())
                        m_AircraftController->updateAttitudeTopic(*component.get());
                }
                else if(components.at(i)->name == DataStateTopic::StateGlobalPositionTopic::Name()) {
                    std::shared_ptr<DataStateTopic::StateGlobalPositionTopic> component = std::make_shared<DataStateTopic::StateGlobalPositionTopic>();
                    m_VehicleDataTopic.GetComponent(component, topicDatagram);
                    if(checkControllerState())
                        m_AircraftController->updateGlobalPositionTopic(*component.get());
                }
                */

                //notify listneres of topic
                ModuleVehicleMavlinkBase::NotifyListenersOfTopic([&](MaceCore::IModuleTopicEvents* ptr){
                    ptr->NewTopicDataValues(this, m_VehicleDataTopic.Name(), systemID, MaceCore::TIME(), topicDatagram);
                });
            }
        } //if there is information available
    }
}

void ModuleVehicleArdupilot::NewTopic(const std::string &topicName, int senderID, std::vector<std::string> &componentsUpdated)
{
    if(topicName == m_VehicleMission.Name())
    {
        MaceCore::TopicDatagram read_topicDatagram = this->getDataObject()->GetCurrentTopicDatagram(m_VehicleMission.Name(), senderID);
        for(size_t i = 0 ; i < componentsUpdated.size() ; i++){
            if(componentsUpdated.at(i) == MissionTopic::MissionItemTopic::Name()) {
                std::shared_ptr<MissionTopic::MissionItemTopic> component = std::make_shared<MissionTopic::MissionItemTopic>();
                m_VehicleMission.GetComponent(component, read_topicDatagram);
            }
            else if(componentsUpdated.at(i) == MissionTopic::MissionListTopic::Name()){
                std::shared_ptr<MissionTopic::MissionListTopic> component = std::make_shared<MissionTopic::MissionListTopic>();
                m_VehicleMission.GetComponent(component, read_topicDatagram);
            }
        }
    }
}

