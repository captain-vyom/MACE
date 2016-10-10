#define MAVLINK_NEED_BYTE_SWAP

#include "module_vehicle_mavlink.h"

#include "mace_core/module_factory.h"

#include <QSerialPort>

#include "comms/serial_link.h"
#include "comms/protocol_mavlink.h"


/*
 *
 * EXAMPLE ON HOW TO GENERATE MAVLINK MESSAGE:
 *
 *   uint8_t chan = m_LinkMarshler->GetProtocolChannel(link);
 *   mavlink_message_t msg;
 *   mavlink_msg_log_request_list_pack_chan(255,190, chan,&msg,0,0,0,0xFFFF);
 *   m_LinkMarshler->SendMessage<mavlink_message_t>(link, msg);
 *
 * */


////////////////////////////////////////////////////////////////////////////////////////////////////////
///             CONFIGURE
////////////////////////////////////////////////////////////////////////////////////////////////////////


ModuleVehicleMAVLINK::ModuleVehicleMAVLINK() :
    MaceCore::IModuleCommandVehicle(),
    m_LinkMarshler(new Comms::CommsMarshaler)
{
    m_LinkMarshler->AddSubscriber(this);
}


//!
//! \brief Describes the strucure of the parameters for this module
//! \return Strucure
//!
std::shared_ptr<MaceCore::ModuleParameterStructure> ModuleVehicleMAVLINK::ModuleConfigurationStructure() const
{
    MaceCore::ModuleParameterStructure structure;

    std::shared_ptr<MaceCore::ModuleParameterStructure> serialSettings = std::make_shared<MaceCore::ModuleParameterStructure>();
    serialSettings->AddTerminalParameters("PortName", MaceCore::ModuleParameterTerminalTypes::STRING, true);
    serialSettings->AddTerminalParameters("BaudRate", MaceCore::ModuleParameterTerminalTypes::INT, true);
    serialSettings->AddTerminalParameters("DataBits", MaceCore::ModuleParameterTerminalTypes::INT, true);
    serialSettings->AddTerminalParameters("StopBits", MaceCore::ModuleParameterTerminalTypes::INT, true);
    serialSettings->AddTerminalParameters("Parity", MaceCore::ModuleParameterTerminalTypes::BOOLEAN, true);
    serialSettings->AddTerminalParameters("FlowControl", MaceCore::ModuleParameterTerminalTypes::INT, true);

    std::shared_ptr<MaceCore::ModuleParameterStructure> protocolSettings = std::make_shared<MaceCore::ModuleParameterStructure>();
    protocolSettings->AddTerminalParameters("Name", MaceCore::ModuleParameterTerminalTypes::STRING, true, "Mavlink", {"Mavlink"});
    protocolSettings->AddTerminalParameters("Version", MaceCore::ModuleParameterTerminalTypes::STRING, true, "V1", {"V1", "V2"});

    structure.AddNonTerminal("SerialParameters", serialSettings, true);
    structure.AddNonTerminal("ProtocolParameters", protocolSettings, true);

    return std::make_shared<MaceCore::ModuleParameterStructure>(structure);
}


//!
//! \brief Provides object contains parameters values to configure module with
//! \param params Parameters to configure
//!
void ModuleVehicleMAVLINK::ConfigureModule(const std::shared_ptr<MaceCore::ModuleParameterValue> &params)
{
    std::shared_ptr<Comms::ProtocolConfiguration> protocolConfig;
    if(params->HasNonTerminal("ProtocolParameters"))
    {
        std::shared_ptr<MaceCore::ModuleParameterValue> protocolSettings = params->GetNonTerminalValue("ProtocolParameters");
        std::string protocolName = protocolSettings->GetTerminalValue<std::string>("Name");
        std::string versionName = protocolSettings->GetTerminalValue<std::string>("Version");


        if(protocolName == "Mavlink")
        {
            std::shared_ptr<Comms::MavlinkConfiguration> mavlinkConfig = std::make_shared<Comms::MavlinkConfiguration>();

            if(versionName == "V1")
            {
                mavlinkConfig->SetVersion(Comms::MavlinkConfiguration::MavlinkVersion::MavlinkVersionAlways1);
            }
            else if(versionName == "V2")
            {
                mavlinkConfig->SetVersion(Comms::MavlinkConfiguration::MavlinkVersion::MavlinkVersionAlways2);
            }
            else
            {
                throw std::runtime_error("Unknown mavlink version seen");
            }

            m_LinkMarshler->AddProtocol(*mavlinkConfig);

            m_AvailableProtocols.insert({Comms::Protocols::MAVLINK, std::static_pointer_cast<Comms::ProtocolConfiguration>(mavlinkConfig)});
            protocolConfig = mavlinkConfig;
        }
        else
        {
            throw std::runtime_error("Unknown Protocol encountered");
        }

    }
    if(params->HasNonTerminal("SerialParameters"))
    {
        std::shared_ptr<MaceCore::ModuleParameterValue> serialSettings = params->GetNonTerminalValue("SerialParameters");


        std::string portName = serialSettings->GetTerminalValue<std::string>("PortName");
        int baudRate = serialSettings->GetTerminalValue<int>("BaudRate");
        int dataBits = serialSettings->GetTerminalValue<int>("DataBits");
        int stopBits = serialSettings->GetTerminalValue<int>("StopBits");

        bool parity = serialSettings->GetTerminalValue<bool>("Parity");
        int flowControl = serialSettings->GetTerminalValue<int>("FlowControl");


        Comms::Protocols protocolToUse = Comms::Protocols::MAVLINK;

        Comms::SerialConfiguration config("config");
        config.setPortName(portName);
        config.setBaud(baudRate);
        config.setDataBits(dataBits);
        config.setStopBits(stopBits);
        config.setParity(parity);
        config.setFlowControl(flowControl);

        m_LinkMarshler->AddLink("link1", config);


        //now configure to use link with desired protocol
        if(protocolToUse == Comms::Protocols::MAVLINK)
        {
            m_LinkMarshler->SetProtocolForLink("link1", Comms::Protocols::MAVLINK);

            std::shared_ptr<Comms::MavlinkConfiguration> mavlinkConfig = std::static_pointer_cast<Comms::MavlinkConfiguration>(m_AvailableProtocols.at(Comms::Protocols::MAVLINK));

            //set version on mavlink channel
            // I would prefer to put this in Comms library, but because the mavlinkstatus is static variable, things get messed up when linking
            uint8_t chan = m_LinkMarshler->GetProtocolChannel("link1");
            mavlink_status_t* mavlinkStatus = mavlink_get_channel_status(chan);
            std::cout << mavlinkStatus << std::endl;
            switch (mavlinkConfig->GetVersion()) {
            case Comms::MavlinkConfiguration::MavlinkVersion::MavlinkVersion2IfVehicle2:
                if (mavlinkStatus->flags & MAVLINK_STATUS_FLAG_IN_MAVLINK1) {
                    mavlinkStatus->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
                    break;
                }
                // Fallthrough to set version 2
            case Comms::MavlinkConfiguration::MavlinkVersion::MavlinkVersionAlways2:
                mavlinkStatus->flags &= ~MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
                break;
            default:
            case Comms::MavlinkConfiguration::MavlinkVersion::MavlinkVersionAlways1:
                mavlinkStatus->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
                break;
            }
        }


        //connect link
        m_LinkMarshler->ConnectToLink("link1");


        //test statements that will issue a log_request_list to device
        uint8_t chan = m_LinkMarshler->GetProtocolChannel("link1");
        mavlink_message_t msg;
        mavlink_msg_log_request_list_pack_chan(255,190, chan,&msg,0,0,0,0xFFFF);
        m_LinkMarshler->SendMessage<mavlink_message_t>("link1", msg);

        //mavlink_msg_request_data_stream_pack_chan(255,190,chan,&msg,0,0,10,4,1);
        //m_LinkMarshler->SendMessage<mavlink_message_t>("link1", msg);

        //param 1 is the message id
        //interval between two messages in microseconds

        //mavlink_msg_command_long_pack_chan(255,190,chan,&msg,0,0,511,0,30,500000,0,0,0,0,0);
        //m_LinkMarshler->SendMessage<mavlink_message_t>("link1", msg);


//        mavlink_msg_request_data_stream_pack_chan(255,190,chan,&msg,0,0,11,4,1);
//        m_LinkMarshler->SendMessage<mavlink_message_t>("link1", msg);

//        mavlink_msg_request_data_stream_pack_chan(255,190,chan,&msg,0,0,12,4,1);
//        m_LinkMarshler->SendMessage<mavlink_message_t>("link1", msg);


    }
    else
    {
        throw std::runtime_error("No Link has been configured for the vehicle MAVLINK module");
    }

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
///              MACE COMMANDS
////////////////////////////////////////////////////////////////////////////////////////////////////////


//!
//! \brief New commands have been updated that the vehicle is to follow immediatly
//!
//! Commands are to be retreived through the MaceData available through getDataObject()
//!
void ModuleVehicleMAVLINK::FollowNewCommands()
{

}


//!
//! \brief New commands have been issued to vehicle that are to be followed once current command is finished
//!
//! Commands are to be retreived through the MaceData available through getDataObject()
//!
void ModuleVehicleMAVLINK::FinishAndFollowNewCommands()
{

}


//!
//! \brief New commands have been appended to existing commands.
//!
//! Commands are to be retreived through the MaceData available through getDataObject()
//!
void ModuleVehicleMAVLINK::CommandsAppended()
{

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
///              COMM EVENTS
////////////////////////////////////////////////////////////////////////////////////////////////////////


//!
//! \brief A Message has been received over Mavlink protocol
//! \param linkName Link identifier which generated call
//! \param message Message that has been received
//!
void ModuleVehicleMAVLINK::MavlinkMessage(const std::string &linkName, const mavlink_message_t &message) const
{
    switch (message.msgid)
    {
    case MAVLINK_MSG_ID_BATTERY_STATUS:
    {
        std::cout << "A battery message was seen" << std::endl;
        mavlink_battery_status_t decodedMSG;
        mavlink_msg_battery_status_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_GPS_RAW_INT:
    {
        std::cout << "A gps raw message was seen" << std::endl;
        mavlink_gps_raw_int_t decodedMSG;
        mavlink_msg_gps_raw_int_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_HEARTBEAT:
    {
        std::cout << "A heartbeat message was seen" << std::endl;
        std::cout<<"The aircraft id is: "<<(int)message.sysid<<std::endl;
        mavlink_heartbeat_t decodedMSG;
        mavlink_msg_heartbeat_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_ATTITUDE:
    {
        std::cout << "An attitude message was seen" << std::endl;
        mavlink_attitude_t decodedMSG;
        mavlink_msg_attitude_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION:
    {
        std::cout << "A home message was seen" << std::endl;
        mavlink_home_position_t decodedMSG;
        mavlink_msg_home_position_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_VFR_HUD:
    {
        std::cout << "A vfr hud message was seen" << std::endl;
        mavlink_vfr_hud_t decodedMSG;
        mavlink_msg_vfr_hud_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_RADIO_STATUS:
    {
        std::cout << "A vfr hud message was seen" << std::endl;
        mavlink_radio_status_t decodedMSG;
        mavlink_msg_radio_status_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    {
        std::cout << "A global position message was seen" << std::endl;
        mavlink_global_position_int_t decodedMSG;
        mavlink_msg_global_position_int_decode(&message,&decodedMSG);
        break;
    }
    default:
        std::cout<<"I saw a message with the ID"<<message.msgid<<std::endl;
    }
    Eigen::Vector3d tmpVector;
    NotifyListeners([&](MaceCore::IModuleEventsVehicle* ptr){
            ptr->NewPositionDynamics(this,MaceCore::TIME(), tmpVector ,tmpVector);
        });
}


//!
//! \brief New heartbeat from MAVLINK received over a link
//! \param linkName Name of link
//! \param vehicleId
//! \param vehicleMavlinkVersion
//! \param vehicleFirmwareType
//! \param vehicleType
//!
void ModuleVehicleMAVLINK::VehicleHeartbeatInfo(const std::string &linkName, int vehicleId, int vehicleMavlinkVersion, int vehicleFirmwareType, int vehicleType) const
{
    //incomming heartbeats
}

