#include "controller_mission.h"

#include "data_interface_MACE/COMMS_to_MACE/helper_mission_comms_to_mace.h"
#include "data_interface_MACE/MACE_to_COMMS/helper_mission_mace_to_comms.h"

namespace ExternalLink {





    //!
    //! \brief Called when building mavlink packet initial request to a mission
    //! \param data
    //! \param cmd
    //!
    bool ControllerMission::Construct_Send(const MissionItem::MissionKey &data, const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target, mace_mission_request_list_t &cmd, MissionItem::MissionKey &queueObj)
    {
        queueObj = data;

        cmd.mission_creator = data.m_creatorID;
        cmd.mission_id = data.m_missionID;
        cmd.mission_system = data.m_systemID;
        cmd.mission_type = (uint8_t)data.m_missionType;
        cmd.mission_state = (uint8_t)data.m_missionState;

        if(m_MissionsBeingFetching.find(data) != m_MissionsBeingFetching.cend())
        {
            throw std::runtime_error("Mission is already being downloaded");
            return false;
        }

        MissionItem::MissionList newList;
        newList.setMissionKey(data);
        newList.clearQueue();
        MissionRequestStruct newItem;
        newItem.mission = newList;
        newItem.requester = sender;
        m_MissionsBeingFetching.insert({data, newItem});

        std::cout << "Mission Controller: Sending Mission Request List" << std::endl;

        return true;
    }

    bool ControllerMission::BuildData_Send(const mace_mission_request_list_t &cmd, const MaceCore::ModuleCharacteristic &sender, mace_mission_count_t &rtn, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &receiveQueueObj, MissionItem::MissionKey &respondQueueObj)
    {
        UNUSED(sender);
        MissionItem::MissionKey key(cmd.mission_system, cmd.mission_creator, cmd.mission_id, static_cast<MissionItem::MISSIONTYPE>(cmd.mission_type), static_cast<MissionItem::MISSIONSTATE>(cmd.mission_state));
        receiveQueueObj = key;
        respondQueueObj = key;

        vehicleObj.ID = key.m_systemID;
        vehicleObj.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

        std::vector<std::tuple<MissionKey, MissionList>> missions;
        CONTROLLER_MISSION_TYPE::FetchDataFromKey(key, missions);

        if(missions.size() == 0)
        {
            return false;
        }
        if(missions.size() > 1)
        {
            throw std::runtime_error("Multiple missions assigned to the same key returned, This is a non-op");
        }
        if(std::get<0>(missions.at(0)) != key)
        {
            throw std::runtime_error("Requesting a specific missionkey did not return the same key, This is a non-op");
        }


        if(m_MissionsUploading.find(key) != m_MissionsUploading.cend())
        {
            std::cout << "Mission Upload Progress: The mission that was requested to be transmitted is already being transmitted" << std::endl;
            return false;
        }
        MissionList mission = std::get<1>(missions.at(0));
        m_MissionsUploading.insert({key, mission});


        rtn.count = m_MissionsUploading.at(key).getQueueSize();
        rtn.target_system = m_MissionsUploading.at(key).getVehicleID();
        rtn.mission_system = key.m_systemID;
        rtn.mission_creator = key.m_creatorID;
        rtn.mission_id = key.m_missionID;
        rtn.mission_type = static_cast<MAV_MISSION_TYPE>(key.m_missionType);
        rtn.mission_state = static_cast<MAV_MISSION_STATE>(key.m_missionState);

        std::cout << "Mission Controller: Sending Mission Count" << std::endl;

        return true;
    }








    bool ControllerMission::BuildData_Send(const mace_mission_count_t &mission, const MaceCore::ModuleCharacteristic &sender, mace_mission_request_item_t &request, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &receiveQueueObj, MissionItem::MissionKey &respondQueueObj)
    {
        UNUSED(sender);
        MissionItem::MissionKey key(mission.mission_system,mission.mission_creator,mission.mission_id,static_cast<MissionItem::MISSIONTYPE>(mission.mission_type),static_cast<MissionItem::MISSIONSTATE>(mission.mission_state));
        receiveQueueObj = key;
        respondQueueObj = key;


        if(m_MissionsBeingFetching.find(key) == m_MissionsBeingFetching.cend())
        {
            return false;
        }

        vehicleObj = m_MissionsBeingFetching[key].requester;

        m_MissionsBeingFetching.at(key).mission.initializeQueue(mission.count);

        request.mission_creator = mission.mission_creator;
        request.mission_id = mission.mission_id;
        request.mission_system = mission.mission_system;
        request.mission_type = mission.mission_type;
        request.mission_state = mission.mission_state;
        request.target_system = mission.target_system;
        request.seq = 0;

        std::cout << "Mission Controller: Requesting Item " << 0 << std::endl;

        return true;
    }


    bool ControllerMission::BuildData_Send(const mace_mission_count_t &mission, const MaceCore::ModuleCharacteristic &sender, mace_mission_request_item_t &request, MaceCore::ModuleCharacteristic &vehicleObj, MaceCore::ModuleCharacteristic &receiveQueueObj, MissionItem::MissionKey &respondQueueObj)
    {
        UNUSED(sender);
        MissionItem::MissionKey key(mission.mission_system,mission.mission_creator,mission.mission_id,static_cast<MissionItem::MISSIONTYPE>(mission.mission_type),static_cast<MissionItem::MISSIONSTATE>(mission.mission_state));
        receiveQueueObj = sender;
        respondQueueObj = key;


        if(m_MissionsBeingFetching.find(key) != m_MissionsBeingFetching.cend())
        {
            return false;
        }

        if(m_GenericRequester.IsSet() == false) {
            throw std::runtime_error("Do not know what module requested a mission");
        }

        MissionItem::MissionList newList;
        newList.setMissionKey(key);
        newList.clearQueue();
        MissionRequestStruct newItem;
        newItem.mission = newList;
        newItem.requester = m_GenericRequester();
        m_MissionsBeingFetching.insert({key, newItem});

        m_GenericRequester = OptionalParameter<MaceCore::ModuleCharacteristic>();

        vehicleObj = m_MissionsBeingFetching[key].requester;

        m_MissionsBeingFetching.at(key).mission.initializeQueue(mission.count);

        request.mission_creator = mission.mission_creator;
        request.mission_id = mission.mission_id;
        request.mission_system = mission.mission_system;
        request.mission_type = mission.mission_type;
        request.mission_state = mission.mission_state;
        request.target_system = mission.target_system;
        request.seq = 0;

        std::cout << "Mission Controller: Requesting Item " << 0 << std::endl;

        return true;
    }






    bool ControllerMission::BuildData_Send(const mace_mission_request_item_t &missionRequest, const MaceCore::ModuleCharacteristic &sender, mace_mission_item_t &missionItem, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &receiveQueueObj, MissionItem::MissionKey &respondQueueObj)
    {
        UNUSED(sender);
        MissionItem::MissionKey key(missionRequest.mission_system,missionRequest.mission_creator,missionRequest.mission_id,static_cast<MissionItem::MISSIONTYPE>(missionRequest.mission_type),static_cast<MissionItem::MISSIONSTATE>(missionRequest.mission_state));
        receiveQueueObj = key;
        respondQueueObj = key;

        vehicleObj.ID = key.m_systemID;
        vehicleObj.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

        if(m_MissionsUploading.find(key) == m_MissionsUploading.cend())
        {
            if(CONTROLLER_MISSION_TYPE::mLog)
                CONTROLLER_MISSION_TYPE::mLog->error("MissionController_ExternalLink has been told to transmit a mission item from a mission which keys dont match the contained.");
            return false;
        }

        int index = missionRequest.seq;
        if(index >= m_MissionsUploading[key].getQueueSize())
        {
            //this indicates that RX system requested something OOR
            if(CONTROLLER_MISSION_TYPE::mLog)
                CONTROLLER_MISSION_TYPE::mLog->error("MissionController_ExternalLink has been told to transmit a mission item with index " + std::to_string(index) + " which is greater than the size of the list contained.");
            return false;
        }

        if(CONTROLLER_MISSION_TYPE::mLog)
            CONTROLLER_MISSION_TYPE::mLog->info("MissionController_ExternalLink has been told to transmit a mission item with index " + std::to_string(index) + ".");

        std::shared_ptr<CommandItem::AbstractCommandItem> ptrItem = this->m_MissionsUploading[key].getMissionItem(index);

        DataInterface_MACE::Helper_MissionMACEtoCOMMS::MACEMissionToCOMMSMission(ptrItem,index,missionItem);
        DataInterface_MACE::Helper_MissionMACEtoCOMMS::updateMissionKey(key,missionItem);

        std::cout << "Mission Controller: Sending Item " << index << std::endl;

        return true;
    }








    bool ControllerMission::BuildData_Send(const mace_mission_item_t &missionItem, const MaceCore::ModuleCharacteristic &sender, mace_mission_request_item_t &request, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &receiveQueueObj, MissionItem::MissionKey &respondQueueObj)
    {
        UNUSED(sender);
        MaceCore::ModuleCharacteristic target;
        target.ID = missionItem.target_system;
        target.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

        MissionItem::MissionKey key(missionItem.target_system,missionItem.mission_creator,missionItem.mission_id,static_cast<MissionItem::MISSIONTYPE>(missionItem.mission_type),static_cast<MissionItem::MISSIONSTATE>(missionItem.mission_state));
        receiveQueueObj = key;
        respondQueueObj = key;

        vehicleObj = m_MissionsBeingFetching[key].requester;

        //check if mission item received is part of a mission we are activly downloading
        if(this->m_MissionsBeingFetching.find(key) == m_MissionsBeingFetching.cend())
        {
            if(CONTROLLER_MISSION_TYPE::mLog)
                CONTROLLER_MISSION_TYPE::mLog->error("Mission controller received a mission item with a key that is not equal to the one we were originally told.");
            return false;
        }

        int seqReceived = missionItem.seq;
        if(seqReceived > (m_MissionsBeingFetching[key].mission.getQueueSize() - 1)) //this should never happen
        {
            std::cout << "Mission download Error: received a mission item with an index greater than available in the queue" << std::endl;
            if(CONTROLLER_MISSION_TYPE::mLog)
                CONTROLLER_MISSION_TYPE::mLog->error("Mission controller received a mission item with an index greater than available in the queue.");
            return false;
        }
        //execution will only continue if not last item
        if(seqReceived == (m_MissionsBeingFetching[key].mission.getQueueSize() - 1))
        {
            return false;
        }

        std::shared_ptr<CommandItem::AbstractCommandItem> newMissionItem = DataInterface_MACE::Helper_MissionCOMMStoMACE::Convert_COMMSTOMACE(missionItem, target);
        m_MissionsBeingFetching[key].mission.replaceMissionItemAtIndex(newMissionItem, seqReceived);

        MissionItem::MissionList::MissionListStatus status = m_MissionsBeingFetching[key].mission.getMissionListStatus();
        if(status.state == MissionItem::MissionList::COMPLETE)
        {
            throw std::runtime_error("Still have more items to request, but mission is full");
        }


        int indexRequest = status.remainingItems.at(0);

        request.target_system = target.ID;
        request.mission_creator = key.m_creatorID;
        request.mission_id = key.m_missionID;
        request.mission_system = key.m_systemID;
        request.mission_type = (uint8_t)key.m_missionType;
        request.mission_state = (uint8_t)key.m_missionState;
        request.seq = indexRequest;

        std::cout << "Mission Controller: Requesting Item " << indexRequest << std::endl;

        return true;
    }





    bool ControllerMission::Construct_FinalObjectAndResponse(const mace_mission_item_t &missionItem, const MaceCore::ModuleCharacteristic &sender, mace_mission_ack_t &ackMission, std::shared_ptr<MissionItem::MissionList> &finalList, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &queueObj)
    {
        UNUSED(sender);
        MaceCore::ModuleCharacteristic target;
        target.ID = missionItem.target_system;
        target.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

        MissionItem::MissionKey key(missionItem.target_system,missionItem.mission_creator,missionItem.mission_id,static_cast<MissionItem::MISSIONTYPE>(missionItem.mission_type),static_cast<MissionItem::MISSIONSTATE>(missionItem.mission_state));
        queueObj = key;

        vehicleObj.ID = key.m_systemID;
        vehicleObj.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

        //check if mission item received is part of a mission we are activly downloading
        if(this->m_MissionsBeingFetching.find(key) == m_MissionsBeingFetching.cend())
        {
            if(CONTROLLER_MISSION_TYPE::mLog)
                CONTROLLER_MISSION_TYPE::mLog->error("Mission controller received a mission item with a key that is not equal to the one we were originally told.");
            return false;
        }

        int seqReceived = missionItem.seq;
        if(seqReceived > (m_MissionsBeingFetching[key].mission.getQueueSize() - 1)) //this should never happen
        {
            std::cout << "Mission download Error: received a mission item with an index greater than available in the queue" << std::endl;
            if(CONTROLLER_MISSION_TYPE::mLog)
                CONTROLLER_MISSION_TYPE::mLog->error("Mission controller received a mission item with an index greater than available in the queue.");
            return false;
        }

        //execution will only continue if last item
        if(seqReceived < (m_MissionsBeingFetching[key].mission.getQueueSize() - 1))
        {
            return false;
        }

        std::shared_ptr<CommandItem::AbstractCommandItem> newMissionItem = DataInterface_MACE::Helper_MissionCOMMStoMACE::Convert_COMMSTOMACE(missionItem, target);
        m_MissionsBeingFetching[key].mission.replaceMissionItemAtIndex(newMissionItem, seqReceived);

        MissionItem::MissionList::MissionListStatus status = m_MissionsBeingFetching[key].mission.getMissionListStatus();
        if(status.state == MissionItem::MissionList::INCOMPLETE)
        {
            throw std::runtime_error("Reached end of request but missions are not completed");
        }

        if(CONTROLLER_MISSION_TYPE::mLog)
        {
            std::stringstream buffer;
            buffer << key;
            CONTROLLER_MISSION_TYPE::mLog->info("Mission Controller has received the entire mission of " + std::to_string(m_MissionsBeingFetching[key].mission.getQueueSize()) + " for mission " + buffer.str() + ".");
        }

        ackMission.mission_system = key.m_systemID;
        ackMission.mission_creator = key.m_creatorID;
        ackMission.mission_id = key.m_missionID;
        ackMission.mission_type = (uint8_t)key.m_missionType;
        ackMission.prev_mission_state = (uint8_t)key.m_missionState;
        ackMission.mission_result = MAV_MISSION_ACCEPTED;

        //KEN This is a hack but for now
        if(key.m_missionState == MissionItem::MISSIONSTATE::PROPOSED)
        {
            ackMission.cur_mission_state = (uint8_t)MissionItem::MISSIONSTATE::RECEIVED;
            m_MissionsBeingFetching[key].mission.setMissionTXState(MissionItem::MISSIONSTATE::RECEIVED);
        }
        else
        {
            ackMission.cur_mission_state = (uint8_t)key.m_missionState;
        }

        finalList = std::make_shared<MissionItem::MissionList>(m_MissionsBeingFetching[key].mission);
        m_MissionsBeingFetching.erase(key);

        std::cout << "Mission Controller: Sending Final ACK" << std::endl;

        return true;
    }

    bool ControllerMission::Finish_Receive(const mace_mission_ack_t &missionItem, const MaceCore::ModuleCharacteristic &sender, uint8_t & ack, MissionItem::MissionKey &queueObj)
    {
        MissionItem::MissionKey key(missionItem.mission_system, missionItem.mission_creator, missionItem.mission_id, static_cast<MissionItem::MISSIONTYPE>(missionItem.mission_type), static_cast<MissionItem::MISSIONSTATE>(missionItem.cur_mission_state));
        queueObj = key;

        ack = missionItem.mission_result;

        std::cout << "Mission Controller: Received Final ACK" << std::endl;

        return true;
    }


    void ControllerMission::Request_Construct(const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target, mace_mission_request_list_generic_t &msg, MaceCore::ModuleCharacteristic &queueObj)
    {
        UNUSED(sender);
        msg.mission_system = target.ID;
        msg.mission_type = (uint8_t)MissionItem::MISSIONSTATE::CURRENT;
        msg.mission_state = 0;

        m_GenericRequester = sender;

        queueObj = target;

        std::cout << "Mission Controller: Sending mission request" << std::endl;
    }

    bool ControllerMission::BuildData_Send(const mace_mission_request_list_generic_t &msg, const MaceCore::ModuleCharacteristic &sender, mace_mission_count_t &response, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &receiveQueueObj, MissionItem::MissionKey &responseQueueObj)
    {
        MissionItem::MISSIONSTATE state = static_cast<MissionItem::MISSIONSTATE>(msg.mission_state);
        if(state == MissionItem::MISSIONSTATE::CURRENT)
        {
            DataItem<MissionKey, MissionList>::FetchModuleReturn items;

            vehicleObj.ID = msg.mission_system;
            vehicleObj.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

            this->FetchFromModule(vehicleObj, items);

            //no modules reported back!
            if(items.size() == 0)
            {
                throw std::runtime_error("No modules reported back");
            }

            //too many modules reported back!
            if(items.size() > 1)
            {
                throw std::runtime_error("More than one module reported");
            }

            std::vector<std::tuple<MissionKey, MissionList>> vec = std::get<1>(items.at(0));
            if(vec.size() == 0)
            {
                return false;
            }
            if(vec.size() == 1)
            {
                MissionItem::MissionKey key = std::get<0>(vec.at(0));
                receiveQueueObj = key;
                responseQueueObj = key;

                if(m_MissionsUploading.find(key) != m_MissionsUploading.cend())
                {
                    std::cout << "Mission Upload Progress: The mission that was requested to be transmitted is already being transmitted" << std::endl;
                    return false;
                }
                MissionList mission = std::get<1>(vec.at(0));
                m_MissionsUploading.insert({key, mission});


                response.count = m_MissionsUploading.at(key).getQueueSize();
                response.target_system = m_MissionsUploading.at(key).getVehicleID();
                response.mission_system = key.m_systemID;
                response.mission_creator = key.m_creatorID;
                response.mission_id = key.m_missionID;
                response.mission_type = static_cast<MAV_MISSION_TYPE>(key.m_missionType);
                response.mission_state = static_cast<MAV_MISSION_STATE>(key.m_missionState);

                std::cout << "Mission Controller: Sending Mission Count" << std::endl;

                return true;
            }
            if(vec.size() > 1)
            {
                throw std::runtime_error("Multiple missions reported back, this is a non-op");
            }
        }
        return false;
    }


    bool ControllerMission::BuildData_Send(const mace_mission_request_list_generic_t &msg, const MaceCore::ModuleCharacteristic &sender, mace_mission_ack_t &response, MaceCore::ModuleCharacteristic &vehicleObj, MissionItem::MissionKey &receiveQueueObj, MissionItem::MissionKey &respondQueueObj)
    {
        MissionItem::MISSIONSTATE state = static_cast<MissionItem::MISSIONSTATE>(msg.mission_state);
        if(state == MissionItem::MISSIONSTATE::CURRENT)
        {
            DataItem<MissionKey, MissionList>::FetchModuleReturn items;

            vehicleObj.ID = msg.mission_system;
            vehicleObj.Class = MaceCore::ModuleClasses::VEHICLE_COMMS;

            this->FetchFromModule(vehicleObj, items);

            //no modules reported back!
            if(items.size() == 0)
            {
                throw std::runtime_error("No modules reported back");
            }

            //too many modules reported back!
            if(items.size() > 1)
            {
                throw std::runtime_error("More than one module reported");
            }

            std::vector<std::tuple<MissionKey, MissionList>> vec = std::get<1>(items.at(0));
            if(vec.size() == 0)
            {
                response.mission_system = msg.mission_system;
                response.cur_mission_state = msg.mission_state;
                response.mission_result = (uint8_t)MissionItem::MissionACK::MISSION_RESULT::MISSION_RESULT_DOES_NOT_EXIST;

                std::cout << "Mission Controller: Received request list, no missions so sending ack" << std::endl;

                return true;
            }
            if(vec.size() == 1)
            {
                return false;
            }
            if(vec.size() > 1)
            {
                throw std::runtime_error("Multiple missions reported back, this is a non-op");
            }
        }
        return false;
    }

    ControllerMission::ControllerMission(const Controllers::IMessageNotifier<mace_message_t> *cb, Controllers::MessageModuleTransmissionQueue<mace_message_t> *queue, int linkChan) :
        CONTROLLER_MISSION_TYPE(cb, queue, linkChan),
        SendHelper_RequestMissionDownload(this, mace_msg_mission_request_list_encode_chan),
        SendHelper_RequestList(this, mace_msg_mission_request_list_decode, mace_msg_mission_count_encode_chan),
        SendHelper_ReceiveCountRespondItemRequest(this, mace_msg_mission_count_decode, mace_msg_mission_request_item_encode_chan),
        SendHelper_ReceiveCountRespondItemRequest_FromRequest(this, mace_msg_mission_count_decode, mace_msg_mission_request_item_encode_chan),
        SendHelper_RequestItem(this, mace_msg_mission_request_item_decode, mace_msg_mission_item_encode_chan),
        SendHelper_ReceiveItem(this,
                               [this](const mace_mission_request_item_t &A, const MaceCore::ModuleCharacteristic &B, const MissionItem::MissionKey &C, const MaceCore::ModuleCharacteristic &D){SendHelper_ReceiveCountRespondItemRequest::NextTransmission(A,B,C,D);},
                               mace_msg_mission_item_decode),
        SendHelper_Final(this, mace_msg_mission_item_decode, mace_msg_mission_ack_encode_chan),
        SendHelper_FinalFinal(this, mace_msg_mission_ack_decode),
        Action_RequestCurrentMission_Initiate(this, mace_msg_mission_request_list_generic_encode_chan),
        Action_RequestCurrentMission_Response(this,
                                [this](const mace_mission_count_t &A, const MaceCore::ModuleCharacteristic &B, const MissionItem::MissionKey &C, const MaceCore::ModuleCharacteristic &D){SendHelper_RequestList::NextTransmission(A,B,C,D);},
                                mace_msg_mission_request_list_generic_decode),
        Action_RequestCurrentMission_NoMissionResponse(this,
                                [this](const mace_mission_ack_t &A, const MaceCore::ModuleCharacteristic &B, const MissionItem::MissionKey &C, const MaceCore::ModuleCharacteristic &D){SendHelper_Final::FinalResponse(A,B,C,D);},
                                mace_msg_mission_request_list_generic_decode)
    {

    }


    void ControllerMission::RequestMission(const MissionItem::MissionKey &key, const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target)
    {
        SendHelper_RequestMissionDownload::Send(key, sender, target);
    }

    void ControllerMission::RequestCurrentMission(const MaceCore::ModuleCharacteristic &sender, const MaceCore::ModuleCharacteristic &target)
    {
        Action_RequestCurrentMission_Initiate::Request(sender, target);
    }

}