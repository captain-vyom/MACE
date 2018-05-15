#ifndef STATE_LANDING_H
#define STATE_LANDING_H

#include "abstract_root_state.h"

namespace ardupilot{
namespace state{

class State_LandingTransitioning;
class State_LandingDescent;
class State_LandingComplete;
class State_Grounded;
class State_Flight;

class State_Landing : public AbstractRootState
{
public:
    State_Landing(ControllerFactory *controllerFactory);

public:
    AbstractStateArdupilot* getClone() const override;

    void getClone(AbstractStateArdupilot** state) const override;

public:
    hsm::Transition GetTransition() override;

public:
    bool handleCommand(const AbstractCommandItem* command) override;

    void Update() override;

    void OnEnter() override;

    void OnEnter(const AbstractCommandItem* command) override;
};

} //end of namespace ardupilot
} //end of namespace state

#endif // STATE_LANDING_H
