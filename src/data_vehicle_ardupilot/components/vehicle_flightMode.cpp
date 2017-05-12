#include "vehicle_flightMode.h"

namespace DataARDUPILOT
{

VehicleFlightMode::VehicleFlightMode()
{
    availableFM = arducopterFM;
}

int VehicleFlightMode::getFlightModeFromString(const std::string &modeString)
{
    std::map<int,std::string>::iterator it;
    int vehicleModeID = 0;
    for (it=availableFM.begin(); it != availableFM.end(); it++)
    {
        if(it->second == modeString)
        {
            vehicleModeID = it->first;
            return vehicleModeID;
        }
    }
}

void VehicleFlightMode::getAvailableFlightModes(const Data::VehicleTypes &vehicleType, std::map<int, std::string> &availableFM)
{
    UNUSED(vehicleType);
    UNUSED(availableFM);
}

bool VehicleFlightMode::vehicleArmable()
{
    if((flightModeString == "STABILIZE") || (flightModeString == "LOITER"))
        return true;
    return false;
}

void VehicleFlightMode::parseMAVLINK(const mavlink_heartbeat_t &msg)
{
    this->setVehicleTypeFromMAVLINK(msg.type);
    std::string newFlightMode = availableFM.at(msg.custom_mode);
    this->setFlightMode(newFlightMode);
}

void VehicleFlightMode::setVehicleTypeFromMAVLINK(const int &vehicleType)
{
        switch (vehicleType) {
        case MAV_TYPE_FIXED_WING:
        {
            this->availableFM = arduplaneFM;
            break;
        }
        case MAV_TYPE_TRICOPTER:
        case MAV_TYPE_QUADROTOR:
        case MAV_TYPE_HEXAROTOR:
        case MAV_TYPE_OCTOROTOR:
        {
            this->availableFM = arducopterFM;
            break;
        }
        default:
            this->availableFM = arducopterFM;
            break;
        }
}

} //end of namespace DataVehicleArdupilot
