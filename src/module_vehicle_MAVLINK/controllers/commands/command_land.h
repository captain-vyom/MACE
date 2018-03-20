#ifndef COMMAND_LAND_H
#define COMMAND_LAND_H

#include "generic_long_command.h"

#include "data_generic_command_item_topic/command_item_topic_components.h"

#include "mavlink.h"

namespace MAVLINKVehicleControllers {


class CommandLand : public Controller_GenericLongCommand<CommandItem::SpatialLand, MAV_CMD_NAV_LAND>
{
public:
    CommandLand(const Controllers::IMessageNotifier<mavlink_message_t> *cb, Controllers::MessageModuleTransmissionQueue<mavlink_message_t> *queue, int linkChan) :
        Controller_GenericLongCommand<CommandItem::SpatialLand, MAV_CMD_NAV_LAND>(cb, queue, linkChan)
    {

    }

    protected:

    virtual void FillCommand(const CommandItem::SpatialLand &commandItem, mavlink_command_long_t &cmd) const
    {
        cmd.target_system = commandItem.getTargetSystem();
        if(commandItem.position->isCoordinateFrame(Data::CoordinateFrameType::CF_GLOBAL_RELATIVE_ALT))
        {
            cmd.param5 = commandItem.position->getX() * pow(10,7);
            cmd.param6 = commandItem.position->getY() * pow(10,7);
            cmd.param7 = commandItem.position->getZ() * 1000;
        }
    }

    virtual void BuildCommand(const mavlink_command_long_t &message, std::shared_ptr<CommandItem::SpatialLand> data) const
    {
        UNUSED(message);
        UNUSED(data);
    }
};


}

#endif // COMMAND_LAND_H