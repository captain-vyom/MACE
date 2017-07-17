#include "command_controller_mavlink.h"

namespace DataInterface_MAVLINK {


CommandController_MAVLINK::CommandController_MAVLINK(const int &targetID, const int &originatingID):
    systemID(targetID), transmittingID(originatingID),
    mToExit(false), currentRetry(0), maxRetries(3), responseTimeout(5000),\
    currentCommsState(NEUTRAL),
    m_CB(NULL), prevTransmit(NULL)
{
    //mLog = spdlog::get("Log_Vehicle" + std::to_string(this->systemID));
}

void CommandController_MAVLINK::clearPreviousTransmit()
{
    if(prevTransmit)
    {
        delete prevTransmit;
        prevTransmit = NULL;
    }
}

void CommandController_MAVLINK::receivedCommandACK(const mavlink_command_ack_t &cmdACK)
{
    m_LambdasToRun.push_back([this, cmdACK]{
        mTimer.stop();

        switch(cmdACK.result)
        {
        case MAV_RESULT_ACCEPTED:
            std::cout<<"MAV result accepted"<<std::endl;
            break;
        case MAV_RESULT_TEMPORARILY_REJECTED:
            std::cout<<"MAV result rejected"<<std::endl;
            break;
        case MAV_RESULT_DENIED:
            std::cout<<"MAV result denied"<<std::endl;
            break;
        case MAV_RESULT_UNSUPPORTED:
            std::cout<<"MAV result unsupported"<<std::endl;
            break;
        case MAV_RESULT_FAILED:
            std::cout<<"MAV result failed"<<std::endl;
            break;
        default:
            std::cout<<"Uknown ack!"<<std::endl;
        }

        commandItemEnum type = prevTransmit->getType();
        switch(type)
        {
        case(commandItemEnum::COMMAND_INT):
        {
            PreviousCommand<mavlink_command_int_t> *tmp = static_cast<PreviousCommand<mavlink_command_int_t>*>(prevTransmit);
            mavlink_command_int_t prevCMD = tmp->getData();
            if(prevCMD.command == cmdACK.command)
            {
                clearPendingTasks();
                mToExit = true;
                currentCommsState = NEUTRAL;
            }
            break;
        }
        case(commandItemEnum::COMMAND_LONG):
        {
            PreviousCommand<mavlink_command_long_t> *tmp = static_cast<PreviousCommand<mavlink_command_long_t>*>(prevTransmit);
            mavlink_command_long_t prevCMD = tmp->getData();
            if(prevCMD.command == cmdACK.command)
            {
                clearPendingTasks();
                mToExit = true;
                currentCommsState = NEUTRAL;
            }
            break;
        }
        case(commandItemEnum::COMMAND_MODE):
        {
            if(cmdACK.command == MAVLINK_MSG_ID_SET_MODE)
            {
                clearPendingTasks();
                mToExit = true;
                currentCommsState = NEUTRAL;
            }
            break;
        }
        default:
        {

        }
        }
    });
}

mavlink_command_long_t CommandController_MAVLINK::initializeCommandLong()
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
    cmdLong.target_system = systemID;
    cmdLong.target_component = 0;
    return cmdLong;
}

void CommandController_MAVLINK::getSystemHome(const int &compID)
{
    mavlink_command_long_t cmd = initializeCommandLong();
    cmd.command = MAV_CMD_GET_HOME_POSITION;
    cmd.target_system = systemID;
    cmd.target_component = compID;

    clearPreviousTransmit();
    prevTransmit = new PreviousCommand<mavlink_command_long_t>(commandItemEnum::COMMAND_INT, cmd);

    currentRetry = 0;
    mToExit = false;
    this->start();
    mTimer.start();

    m_CB->cbiCommandController_transmitCommand(cmd);
}

void CommandController_MAVLINK::setNewMode(const int &newMode)
{
    mavlink_set_mode_t mode;
    mode.target_system = systemID;
    mode.base_mode = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    mode.custom_mode = newMode;

    clearPreviousTransmit();
    prevTransmit = new PreviousCommand<mavlink_set_mode_t>(commandItemEnum::COMMAND_MODE, mode);

    currentRetry = 0;
    mToExit = false;
    this->start();
    mTimer.start();

    m_CB->cbiCommandController_transmitNewMode(mode);
}

void CommandController_MAVLINK::setHomePosition(const CommandItem::SpatialHome &commandItem, const int &compID)
{
    mavlink_command_long_t cmd = initializeCommandLong();
    if(commandItem.position.isCoordinateFrame(Data::CoordinateFrameType::CF_GLOBAL_RELATIVE_ALT))
    {
        cmd.command = MAV_CMD_DO_SET_HOME;
        cmd.target_system = commandItem.getTargetSystem();
        cmd.target_component = compID;
        cmd.param5 = commandItem.position.getX();
        cmd.param6 = commandItem.position.getY();
        cmd.param7 = commandItem.position.getZ();

        clearPreviousTransmit();
        prevTransmit = new PreviousCommand<mavlink_command_long_t>(commandItemEnum::COMMAND_INT, cmd);

        currentRetry = 0;
        mToExit = false;
        this->start();
        mTimer.start();

        m_CB->cbiCommandController_transmitCommand(cmd);
    }
}

void CommandController_MAVLINK::setSystemArm(const CommandItem::ActionArm &commandItem, const int &compID)
{
    mavlink_command_long_t cmd = initializeCommandLong();
    cmd.command = MAV_CMD_COMPONENT_ARM_DISARM;
    cmd.target_system = commandItem.getTargetSystem();
    cmd.target_component = compID;
    cmd.param1 = commandItem.getRequestArm();

    clearPreviousTransmit();
    prevTransmit = new PreviousCommand<mavlink_command_long_t>(commandItemEnum::COMMAND_INT, cmd);

    currentRetry = 0;
    mToExit = false;
    this->start();
    mTimer.start();

    m_CB->cbiCommandController_transmitCommand(cmd);
}

void CommandController_MAVLINK::setSystemTakeoff(const CommandItem::SpatialTakeoff &commandItem, const int &compID)
{
    mavlink_command_long_t cmd = initializeCommandLong();
    cmd.command = MAV_CMD_NAV_TAKEOFF;
    cmd.target_system = commandItem.getTargetSystem();
    cmd.target_component = compID;
    Data::CoordinateFrameType cf = commandItem.position.getCoordinateFrame();

    if(cf == Data::CoordinateFrameType::CF_GLOBAL_RELATIVE_ALT)
    {
        cmd.param7 = commandItem.position.getZ();
    }

    clearPreviousTransmit();
    prevTransmit = new PreviousCommand<mavlink_command_long_t>(commandItemEnum::COMMAND_INT, cmd);

    currentRetry = 0;
    mToExit = false;
    this->start();
    mTimer.start();

    m_CB->cbiCommandController_transmitCommand(cmd);
}

void CommandController_MAVLINK::setSystemLand(const CommandItem::SpatialLand &commandItem, const int &compID)
{
    mavlink_command_long_t cmd = initializeCommandLong();
    cmd.command = MAV_CMD_NAV_LAND;
    cmd.target_system = commandItem.getTargetSystem();
    cmd.target_component = compID;

    if(commandItem.position.isCoordinateFrame(Data::CoordinateFrameType::CF_GLOBAL_RELATIVE_ALT))
    {
        cmd.param5 = commandItem.position.getX() * pow(10,7);
        cmd.param6 = commandItem.position.getY() * pow(10,7);
        cmd.param7 = commandItem.position.getZ() * 1000;
    }

    clearPreviousTransmit();
    prevTransmit = new PreviousCommand<mavlink_command_long_t>(commandItemEnum::COMMAND_INT, cmd);

    currentRetry = 0;
    mToExit = false;
    this->start();
    mTimer.start();

    m_CB->cbiCommandController_transmitCommand(cmd);
}

void CommandController_MAVLINK::setSystemRTL(const CommandItem::SpatialRTL &commandItem, const int &compID)
{
    mavlink_command_long_t cmd = initializeCommandLong();
    cmd.command = MAV_CMD_NAV_RETURN_TO_LAUNCH;
    cmd.target_system = commandItem.getTargetSystem();
    cmd.target_component = compID;

    clearPreviousTransmit();
    prevTransmit = new PreviousCommand<mavlink_command_long_t>(commandItemEnum::COMMAND_INT, cmd);

    currentRetry = 0;
    mToExit = false;
    this->start();
    mTimer.start();

    m_CB->cbiCommandController_transmitCommand(cmd);
}

void CommandController_MAVLINK::run()
{
    while(true)
    {
        if(mToExit == true) {
            clearPendingTasks();
            clearPreviousTransmit();
            mTimer.stop();
            break;
        }

        this->RunPendingTasks();

        //The current state we can find out how much time has passed.
        //If one of the lambda expressions has fired the clock shoud
        //be reset right at the end, thus making this value small and
        //improbable the next function will fire
        double timeElapsed = mTimer.elapsedMilliseconds();

        if(timeElapsed > responseTimeout)
        {
            commandItemEnum type = prevTransmit->getType();
            currentRetry++;

            switch(currentCommsState)
            {
            case NEUTRAL:
            {
                //This case we should terminate this because there is nothing we should be doing apparently
                clearPreviousTransmit();
                mTimer.stop();
                mToExit = true;
             break;
            }
            case TRANSMITTING:
            {

                if(type == commandItemEnum::COMMAND_INT)
                {
                    PreviousCommand<mavlink_command_int_t> *tmp = static_cast<PreviousCommand<mavlink_command_int_t>*>(prevTransmit);
                    mavlink_command_int_t msgTransmit = tmp->getData();
                    mTimer.start();
                    if(m_CB)
                        m_CB->cbiCommandController_transmitCommand(msgTransmit);
                }
                else if(type == commandItemEnum::COMMAND_LONG)
                {
                    PreviousCommand<mavlink_command_long_t> *tmp = static_cast<PreviousCommand<mavlink_command_long_t>*>(prevTransmit);
                    mavlink_command_long_t msgTransmit = tmp->getData();
                    mTimer.start();
                    if(m_CB)
                        m_CB->cbiCommandController_transmitCommand(msgTransmit);
                }
                else if(type == commandItemEnum::COMMAND_MODE)
                {
                    PreviousCommand<mavlink_set_mode_t> *tmp = static_cast<PreviousCommand<mavlink_set_mode_t>*>(prevTransmit);
                    mavlink_set_mode_t msgTransmit = tmp->getData();
                    mTimer.start();
                    if(m_CB)
                        m_CB->cbiCommandController_transmitNewMode(msgTransmit);
                }
                break;
            }
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


} //end of namespace DataInterface_MAVLINK
