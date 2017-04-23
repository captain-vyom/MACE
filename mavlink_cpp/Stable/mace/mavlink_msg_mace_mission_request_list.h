#pragma once
// MESSAGE MACE_MISSION_REQUEST_LIST PACKING

#define MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST 184

MAVPACKED(
typedef struct __mavlink_mace_mission_request_list_t {
 uint8_t target_system; /*< System ID*/
 uint8_t mission_type; /*< Mission type, see MACE_MISSION_TYPE*/
 uint8_t mission_state; /*< Mission type, see MACE_MISSION_STATE*/
}) mavlink_mace_mission_request_list_t;

#define MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN 3
#define MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN 3
#define MAVLINK_MSG_ID_184_LEN 3
#define MAVLINK_MSG_ID_184_MIN_LEN 3

#define MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC 69
#define MAVLINK_MSG_ID_184_CRC 69



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MACE_MISSION_REQUEST_LIST { \
    184, \
    "MACE_MISSION_REQUEST_LIST", \
    3, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mace_mission_request_list_t, target_system) }, \
         { "mission_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_mace_mission_request_list_t, mission_type) }, \
         { "mission_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_mace_mission_request_list_t, mission_state) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MACE_MISSION_REQUEST_LIST { \
    "MACE_MISSION_REQUEST_LIST", \
    3, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mace_mission_request_list_t, target_system) }, \
         { "mission_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_mace_mission_request_list_t, mission_type) }, \
         { "mission_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_mace_mission_request_list_t, mission_state) }, \
         } \
}
#endif

/**
 * @brief Pack a mace_mission_request_list message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param mission_type Mission type, see MACE_MISSION_TYPE
 * @param mission_state Mission type, see MACE_MISSION_STATE
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mace_mission_request_list_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t mission_type, uint8_t mission_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, mission_type);
    _mav_put_uint8_t(buf, 2, mission_state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN);
#else
    mavlink_mace_mission_request_list_t packet;
    packet.target_system = target_system;
    packet.mission_type = mission_type;
    packet.mission_state = mission_state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
}

/**
 * @brief Pack a mace_mission_request_list message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param mission_type Mission type, see MACE_MISSION_TYPE
 * @param mission_state Mission type, see MACE_MISSION_STATE
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mace_mission_request_list_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t mission_type,uint8_t mission_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, mission_type);
    _mav_put_uint8_t(buf, 2, mission_state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN);
#else
    mavlink_mace_mission_request_list_t packet;
    packet.target_system = target_system;
    packet.mission_type = mission_type;
    packet.mission_state = mission_state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
}

/**
 * @brief Encode a mace_mission_request_list struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mace_mission_request_list C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mace_mission_request_list_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mace_mission_request_list_t* mace_mission_request_list)
{
    return mavlink_msg_mace_mission_request_list_pack(system_id, component_id, msg, mace_mission_request_list->target_system, mace_mission_request_list->mission_type, mace_mission_request_list->mission_state);
}

/**
 * @brief Encode a mace_mission_request_list struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mace_mission_request_list C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mace_mission_request_list_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mace_mission_request_list_t* mace_mission_request_list)
{
    return mavlink_msg_mace_mission_request_list_pack_chan(system_id, component_id, chan, msg, mace_mission_request_list->target_system, mace_mission_request_list->mission_type, mace_mission_request_list->mission_state);
}

/**
 * @brief Send a mace_mission_request_list message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param mission_type Mission type, see MACE_MISSION_TYPE
 * @param mission_state Mission type, see MACE_MISSION_STATE
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mace_mission_request_list_send(mavlink_channel_t chan, uint8_t target_system, uint8_t mission_type, uint8_t mission_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, mission_type);
    _mav_put_uint8_t(buf, 2, mission_state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST, buf, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
#else
    mavlink_mace_mission_request_list_t packet;
    packet.target_system = target_system;
    packet.mission_type = mission_type;
    packet.mission_state = mission_state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST, (const char *)&packet, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
#endif
}

/**
 * @brief Send a mace_mission_request_list message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mace_mission_request_list_send_struct(mavlink_channel_t chan, const mavlink_mace_mission_request_list_t* mace_mission_request_list)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mace_mission_request_list_send(chan, mace_mission_request_list->target_system, mace_mission_request_list->mission_type, mace_mission_request_list->mission_state);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST, (const char *)mace_mission_request_list, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
#endif
}

#if MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mace_mission_request_list_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t mission_type, uint8_t mission_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, mission_type);
    _mav_put_uint8_t(buf, 2, mission_state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST, buf, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
#else
    mavlink_mace_mission_request_list_t *packet = (mavlink_mace_mission_request_list_t *)msgbuf;
    packet->target_system = target_system;
    packet->mission_type = mission_type;
    packet->mission_state = mission_state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST, (const char *)packet, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_CRC);
#endif
}
#endif

#endif

// MESSAGE MACE_MISSION_REQUEST_LIST UNPACKING


/**
 * @brief Get field target_system from mace_mission_request_list message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_mace_mission_request_list_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field mission_type from mace_mission_request_list message
 *
 * @return Mission type, see MACE_MISSION_TYPE
 */
static inline uint8_t mavlink_msg_mace_mission_request_list_get_mission_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field mission_state from mace_mission_request_list message
 *
 * @return Mission type, see MACE_MISSION_STATE
 */
static inline uint8_t mavlink_msg_mace_mission_request_list_get_mission_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Decode a mace_mission_request_list message into a struct
 *
 * @param msg The message to decode
 * @param mace_mission_request_list C-struct to decode the message contents into
 */
static inline void mavlink_msg_mace_mission_request_list_decode(const mavlink_message_t* msg, mavlink_mace_mission_request_list_t* mace_mission_request_list)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mace_mission_request_list->target_system = mavlink_msg_mace_mission_request_list_get_target_system(msg);
    mace_mission_request_list->mission_type = mavlink_msg_mace_mission_request_list_get_mission_type(msg);
    mace_mission_request_list->mission_state = mavlink_msg_mace_mission_request_list_get_mission_state(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN? msg->len : MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN;
        memset(mace_mission_request_list, 0, MAVLINK_MSG_ID_MACE_MISSION_REQUEST_LIST_LEN);
    memcpy(mace_mission_request_list, _MAV_PAYLOAD(msg), len);
#endif
}
