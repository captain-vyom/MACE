#ifndef ACTION_REQUEST_H
#define ACTION_REQUEST_H

#include "action_base.h"

namespace Controllers {


//!
//! \brief Sets up an action to make a request
//!
//! This object is close to ActionSend, but doesn't not accept a data (parameters).
//!
//! When sending an action the transmitt will be queued on the controller until the exepcted response is heard.
//! When the given response type is heard by the expected object the queued transmission will be removed, this can be multiple message types.
//!
//! \template MESSAGE_TYPE Underlaying generic message type that all communication is done through
//! \template CONTROLLER_TYPE Type of controller being used by this action, will be used to queue transmissions.
//! \template QUEUE_TYPE Type of object that will establish uniqueness in the queue.
//!   This ultimatly allows a controller to have two identical messages going out to two entities.
//! \template MSG_TYPE Type of communications messsage that is to be transmitted out
//! \template MESSAGE_ACK_ID List of intenger ID that identifies the message that is to stop transmission
//!
template<typename MESSAGE_TYPE, typename CONTROLLER_TYPE, typename QUEUE_TYPE, typename MSG_TYPE, const int ...MESSAGE_ACK_ID>
class ActionRequest :
        public ActionBase<MESSAGE_TYPE, CONTROLLER_TYPE, MSG_TYPE>
{

    typedef ActionBase<MESSAGE_TYPE, CONTROLLER_TYPE, MSG_TYPE> BASE;
protected:



    //!
    //! \brief Method that is to be implimented for this action that generates message of type MSG_TYPE.
    //! This method also realizes the queue object to identify the transmission
    //!
    //! \param sender Module emitting this action, given in the Send function
    //! \param msg Communications message to send to comms interface
    //! \param queue Queue object to identifiy this tranmissions when ack is returned
    //!
    virtual void Request_Construct(const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target, MSG_TYPE &msg, QUEUE_TYPE &queue) = 0;

public:


    ActionRequest(CONTROLLER_TYPE *controller,
               const std::function<void(uint8_t system_id, uint8_t, uint8_t, MESSAGE_TYPE*, const MSG_TYPE*)> &encode_chan) :
        ActionBase<MESSAGE_TYPE, CONTROLLER_TYPE, MSG_TYPE>(controller, encode_chan, {})
    {

    }


    //!
    //! \brief Request this action on a target
    //! \param sender Module sender of this action
    //! \param target Module this action is targeting
    //!
    void Request(const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target)
    {
        MSG_TYPE cmd;
        QUEUE_TYPE queueObj;
        Request_Construct(sender, target, cmd, queueObj);

        std::vector<int> expectedResponses { { MESSAGE_ACK_ID... } };

        BASE::m_Controller-> template QueueTransmission<QUEUE_TYPE>(queueObj, expectedResponses, [this, cmd, sender, target](){
            BASE::m_Controller-> template EncodeMessage(BASE::m_EncodeChanFunc, cmd, sender, target);
        });
    }
};


template <typename ...T>
using ActionRequest_TargetedWithResponse = ActionRequest<T...>;

}




#endif // ACTION_REQUEST_H
