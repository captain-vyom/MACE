#ifndef I_MAVLINK_PARSER_H
#define I_MAVLINK_PARSER_H

#include "mavlink.h"
#include <vector>
#include <unordered_map>
#include <memory>

#include "altitude_reference_frames.h"


#include "data_vehicle_generic/local_position.h"
#include "data_vehicle_generic/local_velocity.h"
#include "data_vehicle_generic/global_position.h"
#include "data_vehicle_generic/global_velocity.h"

#include "data_vehicle_MAVLINK/Components/gps_status.h"


#include "data/i_topic_component_data_object.h"
#include "data/topic_data_object_collection.h"

namespace DataVehicleMAVLINK
{


class MAVLINKParser
{
public:

    std::vector<std::shared_ptr<Data::ITopicComponentDataObject>> Parse(const mavlink_message_t* message) const{

        MaceCore::TopicDatagram datagram;

        int messageID = (int)message->msgid;
        if(messageID == MAVLINK_MSG_ID_LOCAL_POSITION_NED) {
            mavlink_local_position_ned_t decodedMSG;
            mavlink_msg_local_position_ned_decode(message, &decodedMSG);
            std::shared_ptr<DataVehicleGeneric::LocalPosition> pos_ptr = std::make_shared<DataVehicleGeneric::LocalPosition>();
            std::shared_ptr<DataVehicleGeneric::LocalVelocity> vel_ptr = std::make_shared<DataVehicleGeneric::LocalVelocity>();

            pos_ptr->x = decodedMSG.x;
            pos_ptr->y = decodedMSG.y;
            pos_ptr->z = decodedMSG.z;
            pos_ptr->frame = DataVehicleGeneric::CoordinateFrame::NED;

            vel_ptr->x = decodedMSG.vx;
            vel_ptr->y = decodedMSG.vy;
            vel_ptr->z = decodedMSG.vz;
            vel_ptr->frame = DataVehicleGeneric::CoordinateFrame::NED;

            return {pos_ptr, vel_ptr};
        }

        if(messageID == MAVLINK_MSG_ID_GLOBAL_POSITION_INT) {

            mavlink_global_position_int_t decodedMSG;
            mavlink_msg_global_position_int_decode(message, &decodedMSG);

            std::shared_ptr<DataVehicleGeneric::GlobalPosition> pos_ptr = std::make_shared<DataVehicleGeneric::GlobalPosition>();
            std::shared_ptr<DataVehicleGeneric::GlobalVelocity> vel_ptr = std::make_shared<DataVehicleGeneric::GlobalVelocity>();

            //The filtered global position (e.g. fused GPS and accelerometers). The position is in GPS-frame (right-handed, Z-up). It is designed as scaled integer message since the resolution of float is not sufficient.
            pos_ptr->latitude = decodedMSG.lat / pow(10,7);
            pos_ptr->longitude = decodedMSG.lon / pow(10,7);
            pos_ptr->altitude[(int)DataVehicleMAVLINK::AltitudeReferenceFrames::ASL] = decodedMSG.alt  / 1000.0;
            pos_ptr->altitude[(int)DataVehicleMAVLINK::AltitudeReferenceFrames::AGL] = decodedMSG.relative_alt  / 1000.0;

            //Positive assuming a NED convention
            vel_ptr->x = decodedMSG.vx / 100.0;
            vel_ptr->y = decodedMSG.vy / 100.0;
            vel_ptr->z = decodedMSG.vz / 100.0;
            vel_ptr->heading = decodedMSG.hdg;
            vel_ptr->frame = DataVehicleGeneric::CoordinateFrame::NED;

            return {pos_ptr, vel_ptr};
        }


        if(messageID == MAVLINK_MSG_ID_GPS_STATUS)
        {
            //This is message definition 25
            //The positioning status, as reported by GPS. This message is intended to display status information about each satellite visible to the receiver. See message GLOBAL_POSITION for the global position estimate. This message can contain information for up to 20 satellites.

            mavlink_gps_status_t decodedMSG;
            mavlink_msg_gps_status_decode(message, &decodedMSG);

            mavlink_gps_raw_int_t msg;

            std::shared_ptr<DataVehicleMAVLINK::GPSStatus> gpsStatus = std::make_shared<DataVehicleMAVLINK::GPSStatus>();

            gpsStatus->numberOfSats = (int)decodedMSG.satellites_visible;
            //TODO: NOT SURE HOW TO GET REST OF FIELDS.

            return {gpsStatus};
        }

        return {};
    }


    /*
    virtual std::vector<mavlink_message_t*> Generate() const{

    }
    */

};

}
#endif // I_MAVLINK_PARSER_H

/*
    switch (messageID)
    {
    case MAVLINK_MSG_ID_LOG_ENTRY:
    {
        mavlink_log_entry_t decodedMSG;
        mavlink_msg_log_entry_decode(&message,&decodedMSG);
        std::cout<<"A log entry message was received! ID: "<<decodedMSG.id<<" Out of: "<<decodedMSG.num_logs<<"With a size of: "<<decodedMSG.size<<std::endl;
        break;
    }
    case MAVLINK_MSG_ID_COMMAND_ACK:
    {
        mavlink_command_ack_t decodedMSG;
        mavlink_msg_command_ack_decode(&message,&decodedMSG);
        std::cout<<"The command acknowledgement came from: "<<decodedMSG.command<<std::endl;
        switch(decodedMSG.result)
        {
            case MAV_RESULT_ACCEPTED:
                std::cout<<"MAV result accepted"<<std::endl;
                break;
            case MAV_RESULT_TEMPORARILY_REJECTED:
                std::cout<<"MAV result rejected"<<std::endl;
                break;
            case MAV_RESULT_DENIED:
                std::cout<<"MAV result denied"<<std::endl;
                break;
            case MAV_RESULT_UNSUPPORTED:
                std::cout<<"MAV result unsupported"<<std::endl;
                break;
            case MAV_RESULT_FAILED:
                std::cout<<"MAV result failed"<<std::endl;
                break;
            default:
                std::cout<<"Uknown ack!"<<std::endl;
        }
        break;
    }
    case MAVLINK_MSG_ID_HEARTBEAT:
    {
        mavlink_heartbeat_t decodedMSG;
        mavlink_msg_heartbeat_decode(&message,&decodedMSG);
        m_FlightMode->setVehicleType(decodedMSG.type);
        m_FlightMode->setFlightMode(decodedMSG.custom_mode);

        counter = counter + 1;
        if(counter > 20){
            this->setVehicleMode("STABILIZE");
        }
        break;
    }
    case MAVLINK_MSG_ID_SYS_STATUS:
    {
        //This is message definition 1
        break;
    }
    case MAVLINK_MSG_ID_SYSTEM_TIME:
    {
        //This is message definition 2
        //The system time is the time of the master clock, typically the computer clock of the main onboard computer.
        mavlink_system_time_t decodedMSG;
        mavlink_msg_system_time_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
    {
        //This is message definition 20
        //Request to read the onboard parameter with the param_id string id. Onboard parameters are stored as key[const char*] -> value[float]. This allows to send a parameter to any other component (such as the GCS) without the need of previous knowledge of possible parameter names. Thus the same GCS can store different parameters for different autopilots. See also http://qgroundcontrol.org/parameter_interface for a full documentation of QGroundControl and IMU code.
        mavlink_param_request_read_t decodedMSG;
        mavlink_msg_param_request_read_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
    {
        //This is message definition 21
        //Request all parameters of this component. After this request, all parameters are emitted.
        mavlink_param_request_list_t decodedMSG;
        mavlink_msg_param_request_list_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_PARAM_VALUE:
    {
        //This is message definition 22
        //Emit the value of a onboard parameter. The inclusion of param_count and param_index in the message allows the recipient to keep track of received parameters and allows him to re-request missing parameters after a loss or timeout.
        mavlink_param_value_t decodedMSG;
        mavlink_msg_param_value_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_PARAM_SET:
    {
        //This is message definition 23
        //Set a parameter value TEMPORARILY to RAM. It will be reset to default on system reboot. Send the ACTION MAV_ACTION_STORAGE_WRITE to PERMANENTLY write the RAM contents to EEPROM. IMPORTANT: The receiving component should acknowledge the new parameter value by sending a param_value message to all communication partners. This will also ensure that multiple GCS all have an up-to-date list of all parameters. If the sending GCS did not receive a PARAM_VALUE message within its timeout time, it should re-send the PARAM_SET message.
        mavlink_param_set_t decodedMSG;
        mavlink_msg_param_set_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_GPS_RAW_INT:
    {
        //This is message definition 24
        //The global position, as returned by the Global Positioning System (GPS). This is NOT the global position estimate of the system, but rather a RAW sensor value. See message GLOBAL_POSITION for the global position estimate. Coordinate frame is right-handed, Z-axis up (GPS frame).
        mavlink_gps_raw_int_t decodedMSG;
        mavlink_msg_gps_raw_int_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_GPS_STATUS:
    {
        //This is message definition 25
        //The positioning status, as reported by GPS. This message is intended to display status information about each satellite visible to the receiver. See message GLOBAL_POSITION for the global position estimate. This message can contain information for up to 20 satellites.
        m_Position->handleMAVLINKMessage(message);
        break;
    }
    case MAVLINK_MSG_ID_RAW_IMU:
    {
        //This is message definition 27
        break;
    }
    case MAVLINK_MSG_ID_SCALED_PRESSURE:
    {
        //This is message definition 29
        break;
    }
    case MAVLINK_MSG_ID_ATTITUDE:
    {
        //This is message definition 30
        //The attitude in the aeronautical frame (right-handed, Z-down, X-front, Y-right).
        mavlink_attitude_t decodedMSG;
        mavlink_msg_attitude_decode(&message,&decodedMSG);
        m_Attitude->updateAttitudeMavlink(decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    {
        m_Position->handleMAVLINKMessage(message);
        //This is message definition 33
        //The filtered global position (e.g. fused GPS and accelerometers). The position is in GPS-frame (right-handed, Z-up). It is designed as scaled integer message since the resolution of float is not sufficient.
        //mavlink_global_position_int_t decodedMSG;
        //mavlink_msg_global_position_int_decode(&message,&decodedMSG);
        break;
    }

    case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
    {
        //This is message definition 35
        break;
    }
    case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
    {
        //This is message definition 36
        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST_PARTIAL_LIST:
    {
        //This is message definition 37
        //Request a partial list of mission items from the system/component. http://qgroundcontrol.org/mavlink/waypoint_protocol. If start and end index are the same, just send one waypoint.
        mavlink_mission_request_partial_list_t decodedMSG;
        mavlink_msg_mission_request_partial_list_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_WRITE_PARTIAL_LIST:
    {
        //This is message definition 38
        //This message is sent to the MAV to write a partial list. If start index == end index, only one item will be transmitted / updated. If the start index is NOT 0 and above the current list size, this request should be REJECTED!
        mavlink_mission_write_partial_list_t decodedMSG;
        mavlink_msg_mission_write_partial_list_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_ITEM:
    {
        //This is message definition 39
        //Message encoding a mission item. This message is emitted to announce the presence of a mission item and to set a mission item on the system. The mission item can be either in x, y, z meters (type: LOCAL) or x:lat, y:lon, z:altitude. Local frame is Z-down, right handed (NED), global frame is Z-up, right handed (ENU). See also http://qgroundcontrol.org/mavlink/waypoint_protocol.
        mavlink_mission_item_t decodedMSG;
        mavlink_msg_mission_item_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST:
    {
        //This is message definition 40
        //Request the information of the mission item with the sequence number seq. The response of the system to this message should be a MISSION_ITEM message. http://qgroundcontrol.org/mavlink/waypoint_protocol
        mavlink_mission_request_t decodedMSG;
        mavlink_msg_mission_request_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_SET_CURRENT:
    {
        //This is message definition 41
        //Set the mission item with sequence number seq as current item. This means that the MAV will continue to this mission item on the shortest path (not following the mission items in-between).
        mavlink_mission_set_current_t decodedMSG;
        mavlink_msg_mission_set_current_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_CURRENT:
    {
        //This is message definition 42
        //Message that announces the sequence number of the current active mission item. The MAV will fly towards this mission item.
        mavlink_mission_current_t decodedMSG;
        mavlink_msg_mission_current_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
    {
        //This is message definition 43
        //Request the overall list of mission items from the system/component.
        mavlink_mission_request_list_t decodedMSG;
        mavlink_msg_mission_request_list_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_COUNT:
    {
        //This is message definition 44
        //This message is emitted as response to MISSION_REQUEST_LIST by the MAV and to initiate a write transaction. The GCS can then request the individual mission item based on the knowledge of the total number of MISSIONs.
        mavlink_mission_count_t decodedMSG;
        mavlink_msg_mission_count_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
    {
        //This is message definition 45
        //Delete all mission items at once.
        mavlink_mission_clear_all_t decodedMSG;
        mavlink_msg_mission_clear_all_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_ITEM_REACHED:
    {
        //This is message definition 46
        //A certain mission item has been reached. The system will either hold this position (or circle on the orbit) or (if the autocontinue on the WP was set) continue to the next MISSION.
        mavlink_mission_item_reached_t decodedMSG;
        mavlink_msg_mission_item_reached_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_ACK:
    {
        //This is message definition 47
        //Ack message during MISSION handling. The type field states if this message is a positive ack (type=0) or if an error happened (type=non-zero).
        mavlink_mission_ack_t decodedMSG;
        mavlink_msg_mission_ack_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST_INT:
    {
        //This is message definition 51
        //Request the information of the mission item with the sequence number seq. The response of the system to this message should be a MISSION_ITEM_INT message. http://qgroundcontrol.org/mavlink/waypoint_protocol
        mavlink_mission_request_int_t decodedMSG;
        mavlink_msg_mission_request_int_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_VFR_HUD:
    {
        //This is message definition 74
        //Metrics typically displayed on a HUD for fixed wing aircraft
        mavlink_vfr_hud_t decodedMSG;
        mavlink_msg_vfr_hud_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_RADIO_STATUS:
    {
        //This is message definition 109
        //Status generated by radio and injected into MAVLink stream.
        mavlink_radio_status_t decodedMSG;
        mavlink_msg_radio_status_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_SCALED_IMU2:
    {
        //This is message definition 116
        break;
    }
    case MAVLINK_MSG_ID_POWER_STATUS:
    {
        //This is message definition 125
        break;
    }
    case MAVLINK_MSG_ID_BATTERY_STATUS:
    {
        //This is message definition 147
        //Battery information
        mavlink_battery_status_t decodedMSG;
        mavlink_msg_battery_status_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_SENSOR_OFFSETS:
    {
        //This is message definition 150
        break;
    }
    case MAVLINK_MSG_ID_MOUNT_STATUS:
    {
        //This is message definition 158
        //
        mavlink_mount_status_t decodedMSG;
        mavlink_msg_mount_status_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_AHRS:{
        //This is message definition 163
        //
        mavlink_ahrs_t decodedMSG;
        mavlink_msg_ahrs_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_RADIO:{
        //This is message definition 166
        //
        mavlink_radio_t decodedMSG;
        mavlink_msg_radio_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_AHRS2:{
        //This is message definition 165
        //
        mavlink_ahrs2_t decodedMSG;
        mavlink_msg_ahrs2_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_HWSTATUS:{
        //This is message definition 178
        //
        mavlink_hwstatus_t decodedMSG;
        mavlink_msg_hwstatus_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_AHRS3:{
        //This is message definition 182
        break;
    }
    case MAVLINK_MSG_ID_EKF_STATUS_REPORT:{
        //This is message definition 193
        //
        mavlink_ekf_status_report_t decodedMSG;
        mavlink_msg_ekf_status_report_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_VIBRATION:
    {
        //This is message definition 241
        //Vibration levels and accelerometer clipping
        mavlink_vibration_t decodedMSG;
        mavlink_msg_vibration_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION:
    {
        //This is message definition 242
        //This message can be requested by sending the MAV_CMD_GET_HOME_POSITION command. The position the system will return to and land on. The position is set automatically by the system during the takeoff in case it was not explicitely set by the operator before or after. The position the system will return to and land on. The global and local positions encode the position in the respective coordinate frames, while the q parameter encodes the orientation of the surface. Under normal conditions it describes the heading and terrain slope, which can be used by the aircraft to adjust the approach. The approach 3D vector describes the point to which the system should fly in normal flight mode and then perform a landing sequence along the vector.
        mavlink_home_position_t decodedMSG;
        mavlink_msg_home_position_decode(&message,&decodedMSG);
        break;
    }
    case MAVLINK_MSG_ID_STATUSTEXT:
    {
        //This is message definition 253
        mavlink_statustext_t decodedMSG;
        mavlink_msg_statustext_decode(&message,&decodedMSG);
        std::cout<<"A new status text: "<<decodedMSG.text<<std::endl;
        break;
    }
    default:
    {
        std::cout << "I saw a message with the ID "<< message.msgid << std::endl;
    }
    }
   */