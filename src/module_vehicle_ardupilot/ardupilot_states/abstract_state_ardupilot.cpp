#include "abstract_state_ardupilot.h"

namespace ardupilot{
namespace state{

AbstractStateArdupilot::AbstractStateArdupilot(const int &timeout, const int &attempts)
{
    controllerQueue = new Controllers::MessageModuleTransmissionQueue<mavlink_message_t>(timeout, attempts);
}

AbstractStateArdupilot::AbstractStateArdupilot(const AbstractStateArdupilot &copy)
{
    this->currentState = copy.currentState;
    this->desiredState = copy.desiredState;
}

ArdupilotFlightState AbstractStateArdupilot::getCurrentState() const
{
    return currentState;
}

ArdupilotFlightState AbstractStateArdupilot::getDesiredState() const
{
    return desiredState;
}

void AbstractStateArdupilot::clearCommand()
{
    if(this->currentCommand)
    {
        delete currentCommand;
        currentCommand = nullptr;
    }
}

void AbstractStateArdupilot::destroyCurrentControllers()
{
    std::unordered_map<std::string, Controllers::IController<mavlink_message_t>*>::iterator it;
    for(it=currentControllers.begin(); it!=currentControllers.end(); ++it)
    {
        delete it->second;
    }
}

void AbstractStateArdupilot::handleMAVLINKMessage(const mavlink_message_t &msg)
{
    bool consumed = false;
    std::unordered_map<std::string, Controllers::IController<mavlink_message_t>*>::iterator it;
    for(it=currentControllers.begin(); it!=currentControllers.end(); ++it)
    {
        Controllers::IController<mavlink_message_t>* obj = it->second;
        consumed = obj->ReceiveMessage(msg);
    }
    if(!consumed)
        Owner().parseMessage(msg);
}

} //end of namespace state
} //end of namespace ardupilot
