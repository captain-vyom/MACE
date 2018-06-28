#ifndef I_MODULE_EVENTS_GENERAL_H
#define I_MODULE_EVENTS_GENERAL_H

#include "base/pose/geodetic_position_3D.h"

#include "data/system_description.h"
#include "data_generic_command_item/command_item_components.h"
#include "data_generic_state_item/state_global_position.h"

#include "abstract_module_base.h"

namespace MaceCore
{

class IModuleEventsGeneral
{
public:

    //!
    //! \brief Event_ForceVehicleDataSync Event to force a data dump of everything available to a vehicle
    //! \param sender Sender module
    //! \param targetSystemID Target ID of event
    //!
    virtual void Event_ForceVehicleDataSync(const ModuleBase* sender, const int &targetSystemID) = 0;

    //!
    //! \brief Event_IssueCommandSystemArm Event to trigger and ARM command
    //! \param sender Sender module
    //! \param command Arm/Disarm action
    //!
    virtual void Event_IssueCommandSystemArm(const ModuleBase* sender, const CommandItem::ActionArm &command) = 0;

    //!
    //! \brief Event_IssueCommandTakeoff Event to trigger a TAKEOFF command
    //! \param sender Sender module
    //! \param command Takeoff command
    //!
    virtual void Event_IssueCommandTakeoff(const ModuleBase* sender, const CommandItem::SpatialTakeoff &command) = 0;

    //!
    //! \brief Event_IssueCommandLand Event to trigger a LAND command
    //! \param sender Sender module
    //! \param command Land command
    //!
    virtual void Event_IssueCommandLand(const ModuleBase* sender, const CommandItem::SpatialLand &command) = 0;

    //!
    //! \brief Event_IssueCommandRTL Event to trigger an RTL command
    //! \param sender Sender module
    //! \param command RTL command
    //!
    virtual void Event_IssueCommandRTL(const ModuleBase* sender, const CommandItem::SpatialRTL &command) = 0;

    //!
    //! \brief Event_IssueMissionCommand Event to trigger a mission command
    //! \param sender Sender module
    //! \param command Mission command
    //!
    virtual void Event_IssueMissionCommand(const ModuleBase* sender, const CommandItem::ActionMissionCommand &command) = 0;

    //!
    //! \brief Event_ChangeSystemMode Event to trigger a mode change
    //! \param sender Sender module
    //! \param command Mode change command
    //!
    virtual void Event_ChangeSystemMode(const ModuleBase *sender, const CommandItem::ActionChangeMode &command) = 0;

    //!
    //! \brief Event_IssueGeneralCommand Event to trigger a general command
    //! \param sender Sender module
    //! \param command General command
    //!
    virtual void Event_IssueGeneralCommand(const ModuleBase* sender, const std::shared_ptr<CommandItem::AbstractCommandItem> &command) = 0;

    //!
    //! \brief Event_GetMission Event to trigger a "get mission" action
    //! \param sender Sender module
    //! \param key Mission key to fetch
    //!
    virtual void Event_GetMission(const void* sender, const MissionItem::MissionKey &key) = 0;

    //!
    //! \brief Event_GetOnboardMission Event to trigger a fetch of the current onboard mission for a vehicle
    //! \param sender Sender module
    //! \param systemID Vehicle ID whose mission we are asking for
    //! \param type Mission type
    //!
    virtual void Event_GetOnboardMission(const void* sender, const int &systemID, const MissionItem::MISSIONTYPE &type) = 0;

    //!
    //! \brief Event_GetCurrentMission Event to trigger a "get current mission" action
    //! \param sender Sender module
    //! \param systemID Vehicle ID whose mission we are asking for
    //!
    virtual void Event_GetCurrentMission(const void* sender, const int &systemID) = 0;

    //!
    //! \brief RequestClearVehicleMission Request a vehicle clear its onboard mission
    //! \param sender Sender module
    //! \param systemID Vehicle ID whose mission we want to clear
    //!
    virtual void RequestClearVehicleMission(const void* sender, const Data::SystemDescription &systemID) = 0;

    //!
    //! \brief RequestVehicleClearGuidedMission Request to clear a vehicle's guided mission
    //! \param sender Sender module
    //! \param vehicleID Vehicle ID whose guided mission we want to clear
    //!
    virtual void RequestVehicleClearGuidedMission(const void* sender, const int &vehicleID) = 0;

    //!
    //! \brief Event_GetHomePosition Event to trigger a fetch of a vehicle's home position
    //! \param sender Sender module
    //! \param vehicleID Vehicle ID whose home position we want
    //!
    virtual void Event_GetHomePosition(const void* sender, const int &vehicleID) = 0;

    //!
    //! \brief Event_SetHomePosition Event to trigger a set home position action
    //! \param sender Sender module
    //! \param vehicleHome New vehicle home position
    //!
    virtual void Event_SetHomePosition(const ModuleBase *sender, const CommandItem::SpatialHome &vehicleHome) = 0;

    //!
    //! \brief Event_SetGridSpacing Event to set a new grid spacing
    //! \param sender Sender module
    //! \param gridSpacing New grid spacing
    //!
    virtual void Event_SetGridSpacing(const void* sender, const double &gridSpacing) = 0;

    /*
     * The following 3 virtual functions are highly dependent currently since they are all going to use the
     * global origin to determine and decipher the local position that are in the boundary lists. This needs to be
     * generalized and fixed.
     */
    //!
    //! \brief Event_SetGlobalOrigin Event to set a new global origin
    //! \param sender Sender module
    //! \param globalHome New global origin position
    //!
    virtual void Event_SetGlobalOrigin(const void* sender, const mace::pose::GeodeticPosition_3D &globalHome) = 0;

    //!
    //! \brief Event_SetBoundary Event to set a new boundary (boundary key determines type of boundary)
    //! \param sender Sender module
    //! \param boundary New boundary list
    //!
    virtual void Event_SetBoundary(const ModuleBase* sender, const BoundaryItem::BoundaryList &boundary) = 0;
};

} //End MaceCore Namespace

#endif // I_MODULE_EVENTS_GENERAL_H
