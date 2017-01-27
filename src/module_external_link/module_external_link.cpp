#include "module_external_link.h"


ModuleExternalLink::ModuleExternalLink() :
    m_VehicleDataTopic("vehicleData")
{
}

//!
//! \brief This module as been attached as a module
//! \param ptr pointer to object that attached this instance to itself
//!
void ModuleExternalLink::AttachedAsModule(MaceCore::IModuleTopicEvents* ptr)
{
    ptr->Subscribe(this, m_VehicleDataTopic.Name());
}

//!
//! \brief Describes the strucure of the parameters for this module
//! \return Strucure
//!
std::shared_ptr<MaceCore::ModuleParameterStructure> ModuleExternalLink::ModuleConfigurationStructure() const
{
    MaceCore::ModuleParameterStructure structure;
    return std::make_shared<MaceCore::ModuleParameterStructure>(structure);
}


//!
//! \brief Provides object contains parameters values to configure module with
//! \param params Parameters to configure
//!
void ModuleExternalLink::ConfigureModule(const std::shared_ptr<MaceCore::ModuleParameterValue> &params)
{

}

void ModuleExternalLink::NewTopic(const std::string &topicName, int senderID, std::vector<std::string> &componentsUpdated)
{
    //example read of vehicle data
    if(topicName == m_VehicleDataTopic.Name())
    {
        //std::cout << "VehicleData topic received for vehicle: " << senderID << std::endl;

        //get latest datagram from mace_data
        MaceCore::TopicDatagram read_topicDatagram = this->getDataObject()->GetCurrentTopicDatagram(m_VehicleDataTopic.Name(), senderID);

        for(size_t i = 0 ; i < componentsUpdated.size() ; i++) {
            std::cout << "  " << componentsUpdated.at(i) << std::endl;
            if(componentsUpdated.at(i) == DataVehicleGeneric::Attitude::Name()) {
                std::shared_ptr<DataVehicleGeneric::Attitude> component = std::make_shared<DataVehicleArdupilot::VehicleOperatingAttitude>();
                m_VehicleDataTopic.GetComponent(component, read_topicDatagram);
                //std::cout << "    Vehicle Attitude: " << component->getRoll() << std::endl;
            }
            if(componentsUpdated.at(i) == DataVehicleArdupilot::VehicleOperatingParameters::Name()) {
                std::shared_ptr<DataVehicleArdupilot::VehicleOperatingParameters> component = std::make_shared<DataVehicleArdupilot::VehicleOperatingParameters>();
                m_VehicleDataTopic.GetComponent(component, read_topicDatagram);
                //std::cout << "    Vehicle Type: " << (int)component->getPlatform() << std::endl;
            }
            if(componentsUpdated.at(i) == DataVehicleArdupilot::VehicleOperatingStatus::Name()) {
                std::shared_ptr<DataVehicleArdupilot::VehicleOperatingStatus> component = std::make_shared<DataVehicleArdupilot::VehicleOperatingStatus>();
                m_VehicleDataTopic.GetComponent(component, read_topicDatagram);
                //std::cout << "    Vehicle Armed: " << component->getVehicleArmed() << std::endl;
            }
            if(componentsUpdated.at(i) == DataVehicleGeneric::GlobalPosition::Name()) {
                std::shared_ptr<DataVehicleGeneric::GlobalPosition> component = std::make_shared<DataVehicleGeneric::GlobalPosition>();
                m_VehicleDataTopic.GetComponent(component, read_topicDatagram);
                //std::cout << "    lat: " << component->latitude << " long: " << component->longitude << std::endl;
            }
        }
    }
}