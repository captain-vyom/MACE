#include "command_vehicle_land.h"

namespace DataVehicleCommands {
template<class T>
CommandTypes CommandVehicleLand<T>::getCommandType() const
{
    return CommandTypes::ACTION;
}

template<class T>
ActionCommandTypes CommandVehicleLand<T>::getActionItemType() const
{
    return ActionCommandTypes::LAND;
}

template<class T>
std::string CommandVehicleLand<T>::getDescription() const
{
    return "This will command the aircraft to land.";
}

template<>
CommandVehicleLand<DataVehicleGeneric::GlobalPosition>::CommandVehicleLand(){
    m_PositionFrame = Data::PositionalFrame::GLOBAL;
}

template<>
CommandVehicleLand<DataVehicleGeneric::LocalPosition>::CommandVehicleLand(){
    m_PositionFrame = Data::PositionalFrame::LOCAL;
}

template <>
void CommandVehicleLand<DataVehicleGeneric::GlobalPosition>::setLocation(const DataVehicleGeneric::GlobalPosition &location)
{
    m_Location.latitude = location.latitude;
    m_Location.longitude = location.longitude;
    m_Location.altitude = location.altitude;
}
template <>
void CommandVehicleLand<DataVehicleGeneric::LocalPosition>::setLocation(const DataVehicleGeneric::LocalPosition &location)
{
    m_Location.x = location.x;
    m_Location.y = location.y;
    m_Location.z = location.z;
}


}

