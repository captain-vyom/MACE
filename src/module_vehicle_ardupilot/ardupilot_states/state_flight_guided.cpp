#include "state_flight_guided.h"

namespace ardupilot{
namespace state{

State_FlightGuided::State_FlightGuided():
    AbstractStateArdupilot(), guidedTimeout(nullptr), currentQueue(nullptr)
{
    guidedTimeout = new GuidedTimeoutController(1000);
    std::cout<<"We are in the constructor of STATE_FLIGHT_GUIDED"<<std::endl;
    currentStateEnum = ArdupilotFlightState::STATE_FLIGHT_GUIDED;
    desiredStateEnum = ArdupilotFlightState::STATE_FLIGHT_GUIDED;

    guidedTimeout->connectTargetCallback(State_FlightGuided::staticCallbackFunction_VehicleTarget,this);
}

void State_FlightGuided::OnExit()
{
    Owner().state->vehicleLocalPosition.RemoveNotifier(this);
    Owner().mission->currentDynamicQueue.RemoveNotifier(this);

    guidedTimeout->stop();
    delete guidedTimeout;

    delete currentQueue;
}

AbstractStateArdupilot* State_FlightGuided::getClone() const
{
    return (new State_FlightGuided(*this));
}

void State_FlightGuided::getClone(AbstractStateArdupilot** state) const
{
    *state = new State_FlightGuided(*this);
}

hsm::Transition State_FlightGuided::GetTransition()
{
    hsm::Transition rtn = hsm::NoTransition();

    if(currentStateEnum != desiredStateEnum)
    {
        //this means we want to chage the state of the vehicle for some reason
        //this could be caused by a command, action sensed by the vehicle, or
        //for various other peripheral reasons
        switch (desiredStateEnum) {
        default:
            std::cout<<"I dont know how we eneded up in this transition state from State_EStop."<<std::endl;
            break;
        }
    }
    return rtn;
}

bool State_FlightGuided::handleCommand(const AbstractCommandItem* command)
{
    switch (command->getCommandType()) {
    case COMMANDITEM::CI_ACT_MISSIONCOMMAND:
    {
        std::cout<<"We saw that we are going to handle the command in guided mode."<<std::endl;
        guidedTimeout->start();
        //Once we get the command that we can go, we need to announce the current mission item

        Owner().mission->currentDynamicQueue.AddNotifier(this,[this]{
    //        std::lock_guard<std::mutex> guard(MUTEXTargetQueue);
            currentQueue = new TargetItem::DynamicMissionQueue(Owner().mission->currentDynamicQueue.get());
            this->initializeNewTargetList();
        });

        Owner().state->vehicleLocalPosition.AddNotifier(this,[this]
        {
            if((currentQueue->getDynamicTargetList()->listSize() > 0) && (currentQueue->getDynamicTargetList()->getNextIncomplete() != nullptr))
            {
                //std::lock_guard<std::mutex> guard(MUTEXTargetQueue);
                DataState::StateLocalPosition CP = Owner().state->vehicleLocalPosition.get();
                mace::pose::CartesianPosition_3D currentPosition(CP.getPositionX(),CP.getPositionY(),CP.getPositionZ());
                //std::cout<<"The current position here is: "<<CP.getPositionX()<<" "<<CP.getPositionY()<<std::endl;
                unsigned int currentTargetIndex = currentQueue->getDynamicTargetList()->getActiveTargetItem();
                //std::cout<<"The active target item here is: "<<currentTargetIndex<<std::endl;
                const TargetItem::DynamicTarget* target = currentQueue->getDynamicTargetList()->getTargetPointerAtIndex(currentTargetIndex);
                double distance = currentPosition.distanceBetween3D(target->getPosition());
                //std::cout<<"The current target position here is: "<<target->getPosition().getXPosition()<<" "<<target->getPosition().getYPosition()<<" "<<target->getPosition().getZPosition()<<std::endl;

                Data::ControllerState guidedState = guidedProgress.updateTargetState(distance);
                handleGuidedState(currentPosition, currentTargetIndex, guidedState, distance);
            }
        });

        MissionItem::MissionKey testKey(1,1,1,MissionItem::MISSIONTYPE::GUIDED);
        TargetItem::DynamicMissionQueue availableQueue(testKey,1);

        TargetItem::DynamicTarget target;
        target.setPosition(mace::pose::CartesianPosition_3D(1000,1000,100));

        availableQueue.getDynamicTargetList()->appendDynamicTarget(target,TargetItem::DynamicTargetStorage::INCOMPLETE);
        Owner().mission->currentDynamicQueue.set(availableQueue);

        break;
    }
    default:
        break;
    }
}

void State_FlightGuided::Update()
{

}

void State_FlightGuided::OnEnter()
{

}

void State_FlightGuided::OnEnter(const AbstractCommandItem *command)
{
    this->OnEnter();
}

void State_FlightGuided::initializeNewTargetList()
{
    MissionItem::MissionKey associatedKey = currentQueue->getAssociatedMissionKey();
    unsigned int associatedIndex = currentQueue->getAssociatedMissionItem();

    MissionItem::MissionItemCurrent currentMissionItem(associatedKey,associatedIndex);
    Owner().getCallbackInterface()->cbi_VehicleMissionItemCurrent(currentMissionItem);

    unsigned int activeTargetIndex = currentQueue->getDynamicTargetList()->getActiveTargetItem();
    const TargetItem::DynamicTarget target = *currentQueue->getDynamicTargetList()->getNextIncomplete();
    guidedTimeout->updateTarget(target);
}

void State_FlightGuided::handleGuidedState(const mace::pose::CartesianPosition_3D currentPosition, const unsigned int currentTargetIndex,
                                           const Data::ControllerState &state, const double targetDistance)
{
    if(state == Data::ControllerState::ACHIEVED)
    {

        const TargetItem::DynamicTarget* newTarget = currentQueue->getDynamicTargetList()->markCompletionState(currentTargetIndex,TargetItem::DynamicTargetStorage::TargetCompletion::COMPLETE);
        if(newTarget == nullptr)
        {
            //if there are no more points in the queue this mission item is completed
            MissionItem::MissionItemAchieved achievement(currentQueue->getAssociatedMissionKey(),currentQueue->getAssociatedMissionItem());
            std::shared_ptr<MissionTopic::MissionItemReachedTopic> ptrMissionTopic = std::make_shared<MissionTopic::MissionItemReachedTopic>(achievement);
            Owner().getCallbackInterface()->cbi_VehicleMissionData(Owner().getMAVLINKID(),ptrMissionTopic);
        }
        else
        {
            unsigned int currentTargetIndex = currentQueue->getDynamicTargetList()->getActiveTargetItem();
            const TargetItem::DynamicTarget* target = currentQueue->getDynamicTargetList()->getTargetPointerAtIndex(currentTargetIndex);
            double distance = currentPosition.distanceBetween3D(target->getPosition());
            Data::ControllerState guidedState = guidedProgress.newTargetItem(distance);
            handleGuidedState(currentPosition, currentTargetIndex, guidedState, distance);
        }
        //advance to the next desired dynamic state
    }
    else //we are either hunting or tracking the state
    {
        if(Owner().mission->vehicleHomePosition.hasBeenSet())
        {
            //these items are going to be in the local coordinate frame relative to the vehicle home
            CommandItem::SpatialHome home = Owner().mission->vehicleHomePosition.get();
            mace::pose::GeodeticPosition_3D homePos(home.getPosition().getX(),home.getPosition().getY(),home.getPosition().getZ());
            const TargetItem::DynamicTarget* target = currentQueue->getDynamicTargetList()->getTargetPointerAtIndex(currentTargetIndex);

            GeodeticPosition_3D targetPosition;
            DynamicsAid::LocalPositionToGlobal(homePos,target->getPosition(),targetPosition);
            Base3DPosition targetPositionCast(targetPosition.getLatitude(),targetPosition.getLongitude(),targetPosition.getAltitude());
            targetPositionCast.setCoordinateFrame(Data::CoordinateFrameType::CF_GLOBAL_RELATIVE_ALT);
            MissionTopic::VehicleTargetTopic currentTarget(Owner().getMAVLINKID(),targetPositionCast, targetDistance);
            Owner().callTargetCallback(currentTarget);
        }

    }
}

} //end of namespace ardupilot
} //end of namespace state

