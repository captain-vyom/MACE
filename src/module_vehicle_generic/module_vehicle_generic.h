#ifndef MODULE_VEHICLE_GENERIC_H
#define MODULE_VEHICLE_GENERIC_H

#include "module_vehicle_generic_global.h"

#include <functional>

#include "common/common.h"

#include "mace_core/i_module_command_vehicle.h"

#include "data_vehicle_generic/i_vehicle_topic_component.h"




class MODULE_VEHICLE_GENERICSHARED_EXPORT ModuleVehicleGeneric : public MaceCore::IModuleCommandVehicle
{

public:
    ModuleVehicleGeneric();

    virtual std::unordered_map<std::string, MaceCore::TopicStructure> GetTopics();

    virtual std::shared_ptr<DataVehicleGeneric::IVehicleTopicComponent> CraftComponent(const std::string &componentName, const MaceCore::TopicDatagram &datagram);

    //void UpdateComponentData(const int &vehicleID, const std::shared_ptr<DataVehicleGeneric::IVehicleTopicComponent> &component);

    /*
    template <typename T>
    std::shared_ptr<MaceCore::ITopicComponentObject> GetCurrentComponent(const int vehicleID) const {
        if(m_Vehicles.find(vehicleID) == m_Vehicles.cend()){
            throw std::runtime_error("Vehicle does not exists");
        }

        std::type_info &info = typeid(T);
        return m_Vehicles.at(vehicleID).at(info.name());
    }
    */

private:

    std::unordered_map<std::string, std::function<DataVehicleGeneric::IVehicleTopicComponent()>> m_Factory;
    //std::unordered_map<int, std::unordered_map<std::string, std::shared_ptr<MaceCore::ITopicComponentObject>>> m_Vehicles;
};

#endif // MODULE_VEHICLE_GENERIC_H
