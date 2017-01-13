#include "module_vehicle_ardupilot.h"

#include "data_vehicle_ardupilot/mavlink_parser_ardupilot.h"

ModuleVehicleArdupilot::ModuleVehicleArdupilot() :
    ModuleVehicleMAVLINK<>()
{
    //Subscribe to topics of intereset

    //Subscribe to vehicle data for testing purposes.
    //Likely wont be here in fully developed MACE (since a vehicle module doesn't really care about this data)
    NotifyListeners([&](MaceCore::IModuleTopicEvents* ptr){
        ptr->Subscribe(this, m_VehicleDataTopic.Name());
    });
}


//!
//! \brief New Mavlink message received over a link
//! \param linkName Name of link message received over
//! \param msg Message received
//!
void ModuleVehicleArdupilot::MavlinkMessage(const std::string &linkName, const mavlink_message_t &message)
{
    //Base::MavlinkMessage(linkName, message);

    int sendersID = (int)message.sysid;
    int messageID = (int)message.msgid;


    //generate topic datagram from given mavlink message
    std::shared_ptr<MaceCore::TopicDatagram> topicDatagram = m_ArduPilotMAVLINKParser.Parse<VehicleDataTopicType>(&message, &m_VehicleDataTopic);


    //if we generated something then notify any attached MaceCore::IModuleTopicEvents listeners
    if(topicDatagram != NULL)
    {
        //notify of new topic datagram
        NotifyListeners([&, topicDatagram](MaceCore::IModuleTopicEvents* ptr){
            ptr->NewTopicDataValues(m_VehicleDataTopic.Name(), 1, MaceCore::TIME(), *topicDatagram);
        });
    }

}


void ModuleVehicleArdupilot::NewTopic(const std::string &topicName, int senderID, std::vector<std::string> &componentsUpdated)
{
    //example read of vehicle data
    if(topicName == m_VehicleDataTopic.Name())
    {
        std::cout << "VehicleData topic received" << std::endl;
    }
    /*
    MaceCore::TopicDatagram read_topicDatagram = ModuleVehicleGeneric<VehicleTopicAdditionalComponents...>::getDataObject()->GetCurrentTopicDatagram(VehicleDataTopicPtr->Name(), 1);
    std::shared_ptr<DataVehicleGeneric::GlobalPosition> position_component = ((Data::TopicDataObjectCollection<DataVehicleGeneric::GlobalPosition>)*VehicleDataTopicPtr).GetComponent(read_topicDatagram);
    if(position_component != NULL) {
        std::cout << position_component->latitude << " " << position_component->longitude << std::endl;
    }
    */
}
