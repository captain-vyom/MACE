#ifndef STATE_UNKNOWN_H
#define STATE_UNKNOWN_H

#include "abstract_state_ardupilot.h"

namespace ardupilot{
namespace state{

class State_Grounded;
class State_Takeoff;
class State_Flight;
class State_Landing;

class State_Unknown : public AbstractStateArdupilot
{
public:
    State_Unknown(ControllerFactory *controllerFactory);

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


#endif // STATE_UNKNOWN_H
