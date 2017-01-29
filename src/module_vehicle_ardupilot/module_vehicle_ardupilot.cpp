#include "module_vehicle_ardupilot.h"

#include "data_vehicle_ardupilot/mavlink_parser_ardupilot.h"

ModuleVehicleArdupilot::ModuleVehicleArdupilot() :
    ModuleVehicleMAVLINK<DATA_VEHICLE_ARDUPILOT_TYPES>(),m_CommandVehicleTopic("commandData")
{
}


//!
//! \brief This module as been attached as a module
//! \param ptr pointer to object that attached this instance to itself
//!
void ModuleVehicleArdupilot::AttachedAsModule(MaceCore::IModuleTopicEvents* ptr)
{
    ptr->Subscribe(this, m_CommandVehicleTopic.Name());
}


//!
//! \brief New Mavlink message received over a link
//! \param linkName Name of link message received over
//! \param msg Message received
//!
void ModuleVehicleArdupilot::MavlinkMessage(const std::string &linkName, const mavlink_message_t &message)
{
    ModuleVehicleMAVLINK<DATA_VEHICLE_ARDUPILOT_TYPES>::MavlinkMessage(linkName, message);

    //generate topic datagram from given mavlink message
    std::vector<std::shared_ptr<Data::ITopicComponentDataObject>> components = m_ArduPilotMAVLINKParser.Parse(&message);


    //procede to send components only if there is 1 or more
    if(components.size() > 0)
    {

        //construct datagram
        MaceCore::TopicDatagram topicDatagram;
        for(size_t i = 0 ; i < components.size() ; i++)
        {
            m_VehicleDataTopic.SetComponent(components.at(i), topicDatagram);
        }

        //notify listneres of topic
        ModuleVehicleMavlinkBase::NotifyListeners([&](MaceCore::IModuleTopicEvents* ptr){
            ptr->NewTopicDataValues(m_VehicleDataTopic.Name(), 1, MaceCore::TIME(), topicDatagram);
        });
    }
}


void ModuleVehicleArdupilot::NewTopic(const std::string &topicName, int senderID, std::vector<std::string> &componentsUpdated)
{
    if(topicName == m_CommandVehicleTopic.Name())
    {
        MaceCore::TopicDatagram read_topicDatagram = this->getDataObject()->GetCurrentTopicDatagram(m_CommandVehicleTopic.Name(), senderID);
        for(size_t i = 0 ; i < componentsUpdated.size() ; i++) {
            if(componentsUpdated.at(i) == DataVehicleCommands::ActionItemTopic::Name()) {
                std::shared_ptr<DataVehicleCommands::ActionItemTopic> component = std::make_shared<DataVehicleCommands::ActionItemTopic>();
                m_CommandVehicleTopic.GetComponent(component, read_topicDatagram);             
                switch(component->getActionItemType())
                {
                case(DataVehicleCommands::ActionItemTypes::CHANGE_MODE):
                {

                    //const DataVehicleCommands::CommandVehicleMode tmpChangeMode = dynamic_cast<DataVehicleCommands::CommandVehicleMode&>(component->getActionItem());
                    //should find a better way to do this
                    if(m_ArduPilotMAVLINKParser.heartbeatUpdated())
                    {
                        DataVehicleCommands::CommandVehicleMode* cmdMode = (DataVehicleCommands::CommandVehicleMode*)component->getActionItem().get();
                        int newMode = m_ArduPilotMAVLINKParser.getFlightModeFromString(cmdMode->getRequestMode());
                        uint8_t chan = m_LinkMarshaler->GetProtocolChannel("link1");
                        mavlink_message_t msg;
                        mavlink_msg_set_mode_pack_chan(255,190,chan,&msg,1,MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,newMode);
                        m_LinkMarshaler->SendMessage<mavlink_message_t>("link1", msg);
                    }
                    break;
                }
                }



            }
        }
    }
}
