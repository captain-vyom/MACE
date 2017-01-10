#include "module_vehicle_generic.h"

#include <functional>

#include "data_vehicle_generic/local_position.h"
#include "data_vehicle_generic/local_velocity.h"

ModuleVehicleGeneric::ModuleVehicleGeneric() :
    MaceCore::IModuleCommandVehicle()
{

}


std::unordered_map<std::string, MaceCore::TopicStructure> ModuleVehicleGeneric::GetTopics() {

    MaceCore::TopicStructure topic;

    template <typename T>
    auto TopicInserter = [&](const std::string &name, const MaceCore::TopicStructure &structure, const std::function<DataVehicleGeneric::IVehicleTopicComponent(const MaceCore::TopicDatagram &datagram)> &func) {
        topic.AddComponent(name, structure, false);
        m_Factory.insert({name, func});
    };

    TopicInserter(DataVehicleGeneric::LocalPosition::Name(), DataVehicleGeneric::LocalPosition::TopicStructure(), [](const MaceCore::TopicDatagram &datagram){return std::make_shared<DataVehicleGeneric::LocalPosition>(datagram);});
    TopicInserter(DataVehicleGeneric::LocalVelocity::Name(), DataVehicleGeneric::LocalVelocity::TopicStructure(), [](const MaceCore::TopicDatagram &datagram){return std::make_shared<DataVehicleGeneric::LocalVelocity>(datagram);});

    return {{"VehicleData", topic}};
}

std::shared_ptr<DataVehicleGeneric::IVehicleTopicComponent> CraftComponent(const std::string &componentName, const MaceCore::TopicDatagram &datagram)
{

}


/*
void ModuleVehicleGeneric::UpdateComponentData(const int &vehicleID, const std::shared_ptr<MaceCore::ITopicComponentObject> &component) {
    if(m_Vehicles.find(vehicleID) == m_Vehicles.cend()) {
        m_Vehicles.insert({vehicleID, {}});
    }
    std::string key = typeid(*component).name();
    m_Vehicles[vehicleID][key] = component;
}
*/
