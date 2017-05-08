#pragma once
// MESSAGE MACE_MISSION_ITEM_REACHED PACKING

#define MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED 193

MAVPACKED(
typedef struct __mavlink_mace_mission_item_reached_t {
 uint16_t seq; /*< Sequence*/
 uint8_t target_system; /*< System ID*/
 uint8_t target_component; /*< Component ID*/
 uint8_t mission_creator; /*< Creator ID*/
 uint8_t mission_id; /*< Mission ID*/
 uint8_t mission_type; /*< Mission type, see MACE_MISSION_TYPE*/
}) mavlink_mace_mission_item_reached_t;

#define MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN 7
#define MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN 7
#define MAVLINK_MSG_ID_193_LEN 7
#define MAVLINK_MSG_ID_193_MIN_LEN 7

#define MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC 150
#define MAVLINK_MSG_ID_193_CRC 150



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MACE_MISSION_ITEM_REACHED { \
    193, \
    "MACE_MISSION_ITEM_REACHED", \
    6, \
    {  { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_mace_mission_item_reached_t, seq) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_mace_mission_item_reached_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_mace_mission_item_reached_t, target_component) }, \
         { "mission_creator", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_mace_mission_item_reached_t, mission_creator) }, \
         { "mission_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_mace_mission_item_reached_t, mission_id) }, \
         { "mission_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_mace_mission_item_reached_t, mission_type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MACE_MISSION_ITEM_REACHED { \
    "MACE_MISSION_ITEM_REACHED", \
    6, \
    {  { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_mace_mission_item_reached_t, seq) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_mace_mission_item_reached_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_mace_mission_item_reached_t, target_component) }, \
         { "mission_creator", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_mace_mission_item_reached_t, mission_creator) }, \
         { "mission_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_mace_mission_item_reached_t, mission_id) }, \
         { "mission_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_mace_mission_item_reached_t, mission_type) }, \
         } \
}
#endif

/**
 * @brief Pack a mace_mission_item_reached message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param mission_creator Creator ID
 * @param mission_id Mission ID
 * @param mission_type Mission type, see MACE_MISSION_TYPE
 * @param seq Sequence
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mace_mission_item_reached_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint8_t mission_creator, uint8_t mission_id, uint8_t mission_type, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);
    _mav_put_uint8_t(buf, 4, mission_creator);
    _mav_put_uint8_t(buf, 5, mission_id);
    _mav_put_uint8_t(buf, 6, mission_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN);
#else
    mavlink_mace_mission_item_reached_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mission_creator = mission_creator;
    packet.mission_id = mission_id;
    packet.mission_type = mission_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
}

/**
 * @brief Pack a mace_mission_item_reached message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 * @param mission_creator Creator ID
 * @param mission_id Mission ID
 * @param mission_type Mission type, see MACE_MISSION_TYPE
 * @param seq Sequence
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mace_mission_item_reached_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint8_t mission_creator,uint8_t mission_id,uint8_t mission_type,uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);
    _mav_put_uint8_t(buf, 4, mission_creator);
    _mav_put_uint8_t(buf, 5, mission_id);
    _mav_put_uint8_t(buf, 6, mission_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN);
#else
    mavlink_mace_mission_item_reached_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mission_creator = mission_creator;
    packet.mission_id = mission_id;
    packet.mission_type = mission_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
}

/**
 * @brief Encode a mace_mission_item_reached struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mace_mission_item_reached C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mace_mission_item_reached_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mace_mission_item_reached_t* mace_mission_item_reached)
{
    return mavlink_msg_mace_mission_item_reached_pack(system_id, component_id, msg, mace_mission_item_reached->target_system, mace_mission_item_reached->target_component, mace_mission_item_reached->mission_creator, mace_mission_item_reached->mission_id, mace_mission_item_reached->mission_type, mace_mission_item_reached->seq);
}

/**
 * @brief Encode a mace_mission_item_reached struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mace_mission_item_reached C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mace_mission_item_reached_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mace_mission_item_reached_t* mace_mission_item_reached)
{
    return mavlink_msg_mace_mission_item_reached_pack_chan(system_id, component_id, chan, msg, mace_mission_item_reached->target_system, mace_mission_item_reached->target_component, mace_mission_item_reached->mission_creator, mace_mission_item_reached->mission_id, mace_mission_item_reached->mission_type, mace_mission_item_reached->seq);
}

/**
 * @brief Send a mace_mission_item_reached message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param mission_creator Creator ID
 * @param mission_id Mission ID
 * @param mission_type Mission type, see MACE_MISSION_TYPE
 * @param seq Sequence
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mace_mission_item_reached_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint8_t mission_creator, uint8_t mission_id, uint8_t mission_type, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);
    _mav_put_uint8_t(buf, 4, mission_creator);
    _mav_put_uint8_t(buf, 5, mission_id);
    _mav_put_uint8_t(buf, 6, mission_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED, buf, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
#else
    mavlink_mace_mission_item_reached_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mission_creator = mission_creator;
    packet.mission_id = mission_id;
    packet.mission_type = mission_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED, (const char *)&packet, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
#endif
}

/**
 * @brief Send a mace_mission_item_reached message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mace_mission_item_reached_send_struct(mavlink_channel_t chan, const mavlink_mace_mission_item_reached_t* mace_mission_item_reached)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mace_mission_item_reached_send(chan, mace_mission_item_reached->target_system, mace_mission_item_reached->target_component, mace_mission_item_reached->mission_creator, mace_mission_item_reached->mission_id, mace_mission_item_reached->mission_type, mace_mission_item_reached->seq);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED, (const char *)mace_mission_item_reached, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
#endif
}

#if MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mace_mission_item_reached_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint8_t mission_creator, uint8_t mission_id, uint8_t mission_type, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);
    _mav_put_uint8_t(buf, 4, mission_creator);
    _mav_put_uint8_t(buf, 5, mission_id);
    _mav_put_uint8_t(buf, 6, mission_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED, buf, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
#else
    mavlink_mace_mission_item_reached_t *packet = (mavlink_mace_mission_item_reached_t *)msgbuf;
    packet->seq = seq;
    packet->target_system = target_system;
    packet->target_component = target_component;
    packet->mission_creator = mission_creator;
    packet->mission_id = mission_id;
    packet->mission_type = mission_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED, (const char *)packet, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_MIN_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_CRC);
#endif
}
#endif

#endif

// MESSAGE MACE_MISSION_ITEM_REACHED UNPACKING


/**
 * @brief Get field target_system from mace_mission_item_reached message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_mace_mission_item_reached_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field target_component from mace_mission_item_reached message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_mace_mission_item_reached_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field mission_creator from mace_mission_item_reached message
 *
 * @return Creator ID
 */
static inline uint8_t mavlink_msg_mace_mission_item_reached_get_mission_creator(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field mission_id from mace_mission_item_reached message
 *
 * @return Mission ID
 */
static inline uint8_t mavlink_msg_mace_mission_item_reached_get_mission_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field mission_type from mace_mission_item_reached message
 *
 * @return Mission type, see MACE_MISSION_TYPE
 */
static inline uint8_t mavlink_msg_mace_mission_item_reached_get_mission_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field seq from mace_mission_item_reached message
 *
 * @return Sequence
 */
static inline uint16_t mavlink_msg_mace_mission_item_reached_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Decode a mace_mission_item_reached message into a struct
 *
 * @param msg The message to decode
 * @param mace_mission_item_reached C-struct to decode the message contents into
 */
static inline void mavlink_msg_mace_mission_item_reached_decode(const mavlink_message_t* msg, mavlink_mace_mission_item_reached_t* mace_mission_item_reached)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mace_mission_item_reached->seq = mavlink_msg_mace_mission_item_reached_get_seq(msg);
    mace_mission_item_reached->target_system = mavlink_msg_mace_mission_item_reached_get_target_system(msg);
    mace_mission_item_reached->target_component = mavlink_msg_mace_mission_item_reached_get_target_component(msg);
    mace_mission_item_reached->mission_creator = mavlink_msg_mace_mission_item_reached_get_mission_creator(msg);
    mace_mission_item_reached->mission_id = mavlink_msg_mace_mission_item_reached_get_mission_id(msg);
    mace_mission_item_reached->mission_type = mavlink_msg_mace_mission_item_reached_get_mission_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN? msg->len : MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN;
        memset(mace_mission_item_reached, 0, MAVLINK_MSG_ID_MACE_MISSION_ITEM_REACHED_LEN);
    memcpy(mace_mission_item_reached, _MAV_PAYLOAD(msg), len);
#endif
}
