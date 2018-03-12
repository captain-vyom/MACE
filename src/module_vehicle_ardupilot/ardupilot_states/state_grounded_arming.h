#ifndef STATE_GROUNDED_ARMING_H
#define STATE_GROUNDED_ARMING_H

#include "abstract_state_ardupilot.h"

namespace ardupilot{
namespace state{

class State_GroundedArming : public AbstractStateArdupilot
{
public:
    State_GroundedArming();

public:
    AbstractStateArdupilot* getClone() const override;

    void getClone(AbstractStateArdupilot** state) const override;

public:
    hsm::Transition GetTransition() override;

public:
    void handleCommand(const AbstractCommandItem* command) override;

    void Update() override;

    void OnEnter() override;

};

} //end of namespace ardupilot
} //end of namespace state

#endif // STATE_GROUNDED_ARMING_H
