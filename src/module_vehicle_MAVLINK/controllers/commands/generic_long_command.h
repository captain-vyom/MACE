#ifndef MODULE_VEHICLE_MAVLINK_BASE_LONG_COMMAND_H
#define MODULE_VEHICLE_MAVLINK_BASE_LONG_COMMAND_H

#include "common/common.h"

#include "controllers/generic_controller_queue_data_with_module.h"

#include "controllers/actions/action_broadcast.h"
#include "controllers/actions/action_send.h"
#include "controllers/actions/action_final_receive_respond.h"
#include "controllers/actions/action_finish.h"

#include "mavlink.h"

namespace MAVLINKVehicleControllers {

template <typename T>
using ActionSend_LongCommand_TargedWithResponse = Controllers::ActionSend<
    mavlink_message_t,
    Controllers::GenericControllerQueueDataWithModule<mavlink_message_t, T>,
    MaceCore::ModuleCharacteristic,
    T,
    mavlink_command_long_t,
    MAVLINK_MSG_ID_COMMAND_ACK
>;

template<typename T>
using ActionFinish_LongCommand = Controllers::ActionFinish<
    mavlink_message_t,
    Controllers::GenericControllerQueueDataWithModule<mavlink_message_t, T>,
    MaceCore::ModuleCharacteristic,
    uint8_t,
    mavlink_command_ack_t,
    MAVLINK_MSG_ID_COMMAND_ACK
>;


template <typename COMMANDDATASTRUCTURE, const int COMMANDTYPE>
class Controller_GenericLongCommand : public Controllers::GenericControllerQueueDataWithModule<mavlink_message_t, COMMANDDATASTRUCTURE>,
        public ActionSend_LongCommand_TargedWithResponse<COMMANDDATASTRUCTURE>,
        public ActionFinish_LongCommand<COMMANDDATASTRUCTURE>
{
private:

    std::unordered_map<MaceCore::ModuleCharacteristic, MaceCore::ModuleCharacteristic> m_CommandRequestedFrom;

protected:

    virtual void FillCommand(const COMMANDDATASTRUCTURE &, mavlink_command_long_t &) const = 0;

    virtual void BuildCommand(const mavlink_command_long_t &, std::shared_ptr<COMMANDDATASTRUCTURE>) const= 0;

protected:

    virtual bool Construct_Send(const COMMANDDATASTRUCTURE &data, const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target, mavlink_command_long_t &cmd, MaceCore::ModuleCharacteristic &queueObj)
    {
        UNUSED(sender);
        UNUSED(target);
        queueObj.ID = data.getTargetSystem();
        queueObj.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

        cmd = initializeCommandLong();
        cmd.command = COMMANDTYPE;
        cmd.target_system = data.getTargetSystem();
        cmd.target_component = (int)MaceCore::ModuleClasses::VEHICLE_COMMS;

        FillCommand(data, cmd);

        return true;
    }


    virtual bool Finish_Receive(const mavlink_command_ack_t &msg, const MaceCore::ModuleCharacteristic &sender, uint8_t & ack, MaceCore::ModuleCharacteristic &queueObj)
    {
        UNUSED(msg);
        queueObj = sender;
        ack = msg.result;
        return true;
    }

public:

    Controller_GenericLongCommand(const Controllers::IMessageNotifier<mavlink_message_t> *cb, Controllers::MessageModuleTransmissionQueue<mavlink_message_t> *queue, int linkChan) :
        Controllers::GenericControllerQueueDataWithModule<mavlink_message_t, COMMANDDATASTRUCTURE>(cb, queue, linkChan),
        ActionSend_LongCommand_TargedWithResponse<COMMANDDATASTRUCTURE>(this, mavlink_msg_command_long_encode_chan),
        ActionFinish_LongCommand<COMMANDDATASTRUCTURE>(this, mavlink_msg_command_ack_decode)
    {

    }

    virtual ~Controller_GenericLongCommand() = default;


    mavlink_command_long_t initializeCommandLong()
    {
        mavlink_command_long_t cmdLong;
        cmdLong.command = 0;
        cmdLong.confirmation = 0;
        cmdLong.param1 = 0.0;
        cmdLong.param2 = 0.0;
        cmdLong.param3 = 0.0;
        cmdLong.param4 = 0.0;
        cmdLong.param5 = 0.0;
        cmdLong.param6 = 0.0;
        cmdLong.param7 = 0.0;
        cmdLong.target_system = 0;
        cmdLong.target_component = 0;
        return cmdLong;
    }

};

}

#endif // MODULE_VEHICLE_MAVLINK_BASE_LONG_COMMAND_H
