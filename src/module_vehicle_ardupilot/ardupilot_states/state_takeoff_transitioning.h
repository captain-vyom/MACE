#ifndef STATE_TAKEOFF_TRANSITIONING_H
#define STATE_TAKEOFF_TRANSITIONING_H

#include "abstract_state_ardupilot.h"

namespace ardupilot{
namespace state{

class State_TakeoffTransitioning : public AbstractStateArdupilot
{
public:
    State_TakeoffTransitioning();

public:
    AbstractStateArdupilot* getClone() const override;

    void getClone(AbstractStateArdupilot** state) const override;

public:
    hsm::Transition GetTransition() override;

public:
    void handleCommand(const AbstractCommandItem* command) override;

    void Update() override;

    void OnEnter() override;

    void OnEnter(const AbstractCommandItem* command) override;
};

} //end of namespace ardupilot
} //end of namespace state

#endif // STATE_TAKEOFF_TRANSITIONING_H