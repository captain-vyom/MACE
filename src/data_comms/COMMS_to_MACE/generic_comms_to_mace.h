#ifndef GENERIC_COMMS_TO_MACE_H
#define GENERIC_COMMS_TO_MACE_H

#include <math.h>
#include "common/common.h"

#include "mace.h"

#include "data/coordinate_frame.h"
#include "data_generic_item/data_generic_item_components.h"
#include "data_generic_item_topic/data_generic_item_topic_components.h"

namespace DataCOMMS{

class Generic_COMMSTOMACE
{
public:
    Generic_COMMSTOMACE();

    static DataGenericItem::DataGenericItem_SystemArm SystemArm_COMMSTOMACE(const mace_vehicle_armed_t &genericItem, const int &systemID);
    static DataGenericItem::DataGenericItem_FlightMode SystemMode_COMMSTOMACE(const mace_vehicle_mode_t &genericItem, const int &systemID);
    static DataGenericItem::DataGenericItem_Battery Battery_COMMSTOMACE(const mace_battery_status_t &genericItem, const int &systemID);
    static DataGenericItem::DataGenericItem_GPS GPS_COMMSTOMACE(const mace_gps_raw_int_t &genericItem, const int &systemID);
    static DataGenericItem::DataGenericItem_Text Text_COMMSTOMACE(const mace_statustext_t &genericItem, const int &systemID);
};

} //end of namespace DataCOMMS
#endif // GENERIC_COMMS_TO_MACE_H