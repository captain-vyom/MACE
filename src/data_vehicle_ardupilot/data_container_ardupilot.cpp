#include "data_container_ardupilot.h"
#include <iostream>

DataContainer_ARDUPILOT::DataContainer_ARDUPILOT() :
    DataMAVLINK::DataContainer_MAVLINK()
{
    m_ArducopterFlightMode = std::make_shared<DataARDUPILOT::VehicleFlightMode>();
}
