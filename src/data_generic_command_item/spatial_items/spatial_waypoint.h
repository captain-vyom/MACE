#ifndef SPATIAL_WAYPOINT_H
#define SPATIAL_WAYPOINT_H

#include <iostream>
#include <iomanip>
#include <sstream>

#include "data/command_item_type.h"

#include "data_generic_command_item/abstract_command_item.h"

#include "data_generic_state_item/base_3d_position.h"

namespace CommandItem {

class SpatialWaypoint : public AbstractCommandItem
{

public:
    SpatialWaypoint();
    SpatialWaypoint(const SpatialWaypoint &obj);
    SpatialWaypoint(const int &systemOrigin, const int &systemTarget = 0);

    ~SpatialWaypoint();

public:

    //!
    //! \brief getCommandType returns the type of the object that this command type is.
    //! \return Data::CommandType resolving the type of command this object is.
    //!
    virtual Data::CommandItemType getCommandType()const;

    //!
    //! \brief getDescription
    //! \return string describing the command item. This may be useful for setting up options in a
    //! GUI or somewhere a display needs to interface and decisions have to be made describing what
    //! would happen when issuing such a command.
    //!
    virtual std::string getDescription()const;

    //!
    //! \brief hasSpatialInfluence returns a boolean reflecting whether or not the commandItem has
    //! a direct influence over a vehicles position. This is useful for determining flight times,
    //! position elements, or rendering objects on a GUI.
    //! \return false if the command does not have an affect over the vehicles position directly.
    //! For example, change speed has no influence over a vehicles position.
    //!
    virtual bool hasSpatialInfluence()const;

public:
    void operator = (const SpatialWaypoint &rhs)
    {
        AbstractCommandItem::operator =(rhs);
        this->position = rhs.position;
    }

    bool operator == (const SpatialWaypoint &rhs) {
        if(!AbstractCommandItem::operator ==(rhs))
        {
            return false;
        }
        if(this->position != rhs.position)
        {
            return false;
        }
        return true;
    }

    bool operator != (const SpatialWaypoint &rhs) {
        return !(*this == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const SpatialWaypoint& t);

public:
    DataState::Base3DPosition position;

};

} //end of namespace MissionItem
#endif // SPATIAL_WAYPOINT_H
