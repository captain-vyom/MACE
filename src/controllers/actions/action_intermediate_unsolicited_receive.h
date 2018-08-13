#ifndef ACTION_INTERMEDIATE_UNSOLICITED_RECEIVE_H
#define ACTION_INTERMEDIATE_UNSOLICITED_RECEIVE_H

#include "action_base.h"

namespace Controllers {

template<typename MESSAGE_TYPE, typename CONTROLLER_TYPE, typename RESPOND_QUEUE_TYPE, typename MSG_TYPE, const int MESSAGE_REQUEST_ID, typename ACK_TYPE>
class ActionIntermediateUnsolicitedReceive :
        public ActionBase<MESSAGE_TYPE, CONTROLLER_TYPE, MSG_TYPE>
{

    typedef ActionBase<MESSAGE_TYPE, CONTROLLER_TYPE, MSG_TYPE> BASE;

protected:

    virtual bool IntermediateUnsolicitedReceive(const MSG_TYPE &, const MaceCore::ModuleCharacteristic &sender, ACK_TYPE &, MaceCore::ModuleCharacteristic &vehicleObj, RESPOND_QUEUE_TYPE &respondQueueObj)= 0;

public:

    ActionIntermediateUnsolicitedReceive()
    {
        throw std::runtime_error("Default Constructor not supported");
    }

    ActionIntermediateUnsolicitedReceive(CONTROLLER_TYPE *controller,
                                  const std::function<void(const ACK_TYPE &, const MaceCore::ModuleCharacteristic &, const RESPOND_QUEUE_TYPE &, const MaceCore::ModuleCharacteristic &)> &nextStep,
                                  const std::function<void(const MESSAGE_TYPE*, MSG_TYPE*)> &decode) :
        ActionBase<MESSAGE_TYPE, CONTROLLER_TYPE, MSG_TYPE>(controller, {}, decode)
    {


        BASE::m_Controller-> template AddTriggeredLogic<MESSAGE_REQUEST_ID, MSG_TYPE>( BASE::m_DecodeFunc,
                [this, nextStep](const MSG_TYPE  &msg, const MaceCore::ModuleCharacteristic &sender){

                    MaceCore::ModuleCharacteristic target = sender;

                    MaceCore::ModuleCharacteristic vehicleFrom;
                    ACK_TYPE ack;
                    RESPOND_QUEUE_TYPE respondQueueObj;

                    bool valid = this-> template IntermediateUnsolicitedReceive(msg, sender, ack, vehicleFrom, respondQueueObj);


                    if(valid == true)
                    {
                        //BASE::m_Controller->Set(ack, vehicleFrom, queueObj, target);
                        nextStep(ack, vehicleFrom, respondQueueObj, target);
                    }
                }
        );
    }
};

}

#endif // ACTION_INTERMEDIATE_UNSOLICITED_RECEIVE_H