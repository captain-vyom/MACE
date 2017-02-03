#include "command_vehicle_takeoff.h"

namespace DataVehicleCommands {
template<class T>
CommandTypes CommandVehicleTakeoff<T>::getCommandType() const
{
    return CommandTypes::ACTION;
}

template<class T>
ActionCommandTypes CommandVehicleTakeoff<T>::getActionItemType() const
{
    return ActionCommandTypes::TAKEOFF;
}

template<class T>
std::string CommandVehicleTakeoff<T>::getDescription() const
{
    return "This will command the aircraft to takeoff.";
}

template <>
void CommandVehicleTakeoff<DataVehicleGeneric::GlobalPosition>::setLocation(const DataVehicleGeneric::GlobalPosition &location)
{
    m_Location.latitude = location.latitude;
    m_Location.longitude = location.longitude;
    m_Location.altitude = location.altitude;
}
template <>
void CommandVehicleTakeoff<DataVehicleGeneric::LocalPosition>::setLocation(const DataVehicleGeneric::LocalPosition &location)
{
    m_Location.x = location.x;
    m_Location.y = location.y;
    m_Location.z = location.z;
}


}





