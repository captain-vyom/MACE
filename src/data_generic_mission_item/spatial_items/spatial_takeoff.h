#ifndef SPATIAL_TAKEOFF_H
#define SPATIAL_TAKEOFF_H

#include "data_generic_mission_item/abstract_mission_item.h"
#include "data_generic_mission_item/mission_item_types.h"

#include "data_generic_state_item/state_global_position.h"
#include "data_generic_state_item/state_local_position.h"

namespace MissionItem {

template <class T>
class SpatialTakeoff : public AbstractMissionItem
{
public:
    virtual MissionItemType getMissionType()const;

    virtual std::string getDescription()const;

    virtual bool hasSpatialMissionInfluence()const;

public:
    void operator = (const SpatialTakeoff &rhs)
    {
        AbstractMissionItem::operator =(rhs);
        this->position = rhs.position;
    }

    bool operator == (const SpatialTakeoff &rhs) {
        if(!AbstractMissionItem::operator ==(rhs))
        {
            return false;
        }
        if(this->position != rhs.position){
            return false;
        }
        return true;
    }

    bool operator != (const SpatialTakeoff &rhs) {
        return !(*this == rhs);
    }

public:
    T position;
};

} //end of namespace MissionItem

#endif // SPATIAL_TAKEOFF_H