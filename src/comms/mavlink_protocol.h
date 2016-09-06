#ifndef MAVLINK_COMMS_H
#define MAVLINK_COMMS_H

#include "comms_global.h"

#include <memory>
#include <vector>
#include <functional>
#include <math.h>
#include <unordered_map>

#include "mavlink.h"
#include "i_link.h"
#include "i_mavlink_protocol_events.h"

#include "i_protocol.h"

namespace Comms
{

class COMMSSHARED_EXPORT MavlinkComms : public IProtocol
{
public:
    enum class MavlinkVersion
    {
        MavlinkVersion2IfVehicle2,
        MavlinkVersionAlways2,
        MavlinkVersionAlways1
    };

public:

    MavlinkVersion m_version;

public:
    MavlinkComms() :
        m_version(MavlinkComms::MavlinkVersion::MavlinkVersionAlways1)
    {
        memset(&totalReceiveCounter, 0, sizeof(totalReceiveCounter));
        memset(&totalLossCounter, 0, sizeof(totalLossCounter));
        memset(&totalErrorCounter, 0, sizeof(totalErrorCounter));
        memset(&currReceiveCounter, 0, sizeof(currReceiveCounter));
        memset(&currLossCounter, 0, sizeof(currLossCounter));
    }

    void AddListner(const IMavlinkCommsEvents* listener)
    {
        m_Listners.push_back(listener);
    }


    void SetVersion(MavlinkVersion version)
    {
        m_version = version;
    }


    int getSystemId()
    {
        return m_systemId;
    }

    void setSystemId(int id)
    {
        m_systemId = id;
    }

    int getComponentId()
    {
        return 0;
    }


    virtual void ResetMetadataForLink(const ILink* link)
    {
        if(m_MavlinkChannels.find(link) == m_MavlinkChannels.cend())
            return;

        int channel = m_MavlinkChannels.at(link);
        totalReceiveCounter[channel] = 0;
        totalLossCounter[channel] = 0;
        totalErrorCounter[channel] = 0;
        currReceiveCounter[channel] = 0;
        currLossCounter[channel] = 0;
    }


    //!
    //! \brief Get the protocol channel being used for a specific link
    //! \param link Link to check
    //! \return Channel of the protocol being used
    //!
    virtual uint8_t GetChannel(ILink *link) const
    {
        if(m_MavlinkChannels.find(link) == m_MavlinkChannels.cend())
            throw std::runtime_error("Link does not have a channel");

        return m_MavlinkChannels.at(link);
    }


    //!
    //! \brief Set the channel being used for a specific link on the protocol
    //! \param link Link to set
    //! \param channel Channel to use
    //!
    virtual void SetChannel(ILink *link, uint8_t channel)
    {
        m_MavlinkChannels[link] = channel;
    }


    //!
    //! \brief Send message onto some link
    //!
    //! This code is largely a copy from MAVLinkProtocol::sendMessage in qgroundcontrol
    //! \param link Link to put message onto
    //! \param message Message to send
    //!
    void SendMessage(const ILink *link, const mavlink_message_t &message)
    {
        // Create buffer
        static uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        // Write message into buffer, prepending start sign
        int len = mavlink_msg_to_send_buffer(buffer, &message);
        // If link is connected
        if (link->isConnected())
        {
            // Send the portion of the buffer now occupied by the message
            link->WriteBytes((const char*)buffer, len);
        }
    }


    //!
    //! \brief Read data incoming from some link
    //!
    //! This code is largely a copy from MAVLinkProtocol::receiveBytes in qgroundcontrol
    //! \param link Link which data was read from
    //! \param buffer data that was read.
    //!
    virtual void ReceiveData(ILink *link, const std::vector<uint8_t> &buffer)
    {
        uint8_t mavlinkChannel = m_MavlinkChannels.at(link);

        mavlink_message_t message;
        mavlink_status_t status;

        static int mavlink09Count = 0;
        static int nonmavlinkCount = 0;
        static bool decodedFirstPacket = false;
        static bool warnedUser = false;
        static bool checkedUserNonMavlink = false;
        static bool warnedUserNonMavlink = false;

        for(uint8_t c: buffer)
        {
            unsigned int decodeState = mavlink_parse_char(mavlinkChannel, c, &message, &status);

            if (c == 0x55) mavlink09Count++;
            if ((mavlink09Count > 100) && !decodedFirstPacket && !warnedUser)
            {
                warnedUser = true;

                // Obviously the user tries to use a 0.9 autopilot
                // with QGroundControl built for version 1.0
                Emit([](const IMavlinkCommsEvents* ptr)
                {
                    ptr->ProtocolStatusMessage("MAVLINK Protocol", std::string("There is a MAVLink Version or Baud Rate Mismatch. ") +
                                                                   "Your MAVLink device seems to use the deprecated version 0.9, while QGroundControl only supports version 1.0+. " +
                                                                   "Please upgrade the MAVLink version of your autopilot. " +
                                                                   "If your autopilot is using version 1.0, check if the baud rates of QGroundControl and your autopilot are the same.");
                });
            }

            if (decodeState == 0 && !decodedFirstPacket)
            {
                nonmavlinkCount++;
                if (nonmavlinkCount > 2000 && !warnedUserNonMavlink)
                {
                    //2000 bytes with no mavlink message. Are we connected to a mavlink capable device?
                    if (!checkedUserNonMavlink)
                    {
                        link->RequestReset();
                        checkedUserNonMavlink = true;
                    }
                    else
                    {
                        warnedUserNonMavlink = true;
                        Emit([](const IMavlinkCommsEvents* ptr)
                        {
                            ptr->ProtocolStatusMessage("MAVLINK Protocol", std::string("There is a MAVLink Version or Baud Rate Mismatch. ") +
                                                                           "Please check if the baud rates of QGroundControl and your autopilot are the same.");
                        });
                    }
                }
            }
            if (decodeState == 1)
            {
                decodedFirstPacket = true;

                if(message.msgid == MAVLINK_MSG_ID_PING)
                {
                    // process ping requests (tgt_system and tgt_comp must be zero)
                    mavlink_ping_t ping;
                    mavlink_msg_ping_decode(&message, &ping);
                    if(!ping.target_system && !ping.target_component)
                    {
                        mavlink_message_t msg;
                        mavlink_msg_ping_pack(getSystemId(), getComponentId(), &msg, ping.time_usec, ping.seq, message.sysid, message.compid);
                        SendMessage(link, msg);
                    }
                }

                if(message.msgid == MAVLINK_MSG_ID_RADIO_STATUS)
                {
                    // process telemetry status message
                    mavlink_radio_status_t rstatus;
                    mavlink_msg_radio_status_decode(&message, &rstatus);
                    int rssi = rstatus.rssi,
                        remrssi = rstatus.remrssi;
                    // 3DR Si1k radio needs rssi fields to be converted to dBm
                    if (message.sysid == '3' && message.compid == 'D') {
                        /* Per the Si1K datasheet figure 23.25 and SI AN474 code
                         * samples the relationship between the RSSI register
                         * and received power is as follows:
                         *
                         *                       10
                         * inputPower = rssi * ------ 127
                         *                       19
                         *
                         * Additionally limit to the only realistic range [-120,0] dBm
                         */
                        rssi    = std::min(std::max(round(static_cast<float>(rssi)    / 1.9 - 127.0), - 120.0), 0.0);
                        remrssi = std::min(std::max(round(static_cast<float>(remrssi) / 1.9 - 127.0), - 120.0), 0.0);
                    } else {
                        rssi = (int8_t) rstatus.rssi;
                        remrssi = (int8_t) rstatus.remrssi;
                    }

                    Emit([&](const IMavlinkCommsEvents* ptr){ptr->RadioStatusChanged(link, rstatus.rxerrors, rstatus.fixed, rssi, remrssi, rstatus.txbuf, rstatus.noise, rstatus.remnoise);});
                }

                /*
    #ifndef __mobile__
                // Log data

                if (!_logSuspendError && !_logSuspendReplay && _tempLogFile.isOpen()) {
                    uint8_t buf[MAVLINK_MAX_PACKET_LEN+sizeof(quint64)];

                    // Write the uint64 time in microseconds in big endian format before the message.
                    // This timestamp is saved in UTC time. We are only saving in ms precision because
                    // getting more than this isn't possible with Qt without a ton of extra code.
                    uint64_t time = (uint64_t)QDateTime::currentMSecsSinceEpoch() * 1000;
                    qToBigEndian(time, buf);

                    // Then write the message to the buffer
                    int len = mavlink_msg_to_send_buffer(buf + sizeof(quint64), &message);

                    // Determine how many bytes were written by adding the timestamp size to the message size
                    len += sizeof(quint64);

                    // Now write this timestamp/message pair to the log.
                    QByteArray b((const char*)buf, len);
                    if(_tempLogFile.write(b) != len)
                    {
                        // If there's an error logging data, raise an alert and stop logging.
                        emit protocolStatusMessage(tr("MAVLink Protocol"), tr("MAVLink Logging failed. Could not write to file %1, logging disabled.").arg(_tempLogFile.fileName()));
                        _stopLogging();
                        _logSuspendError = true;
                    }

                    // Check for the vehicle arming going by. This is used to trigger log save.
                    if (!_logPromptForSave && message.msgid == MAVLINK_MSG_ID_HEARTBEAT)
                    {
                        mavlink_heartbeat_t state;
                        mavlink_msg_heartbeat_decode(&message, &state);
                        if (state.base_mode & MAV_MODE_FLAG_DECODE_POSITION_SAFETY)
                        {
                            _logPromptForSave = true;
                        }
                    }
                }
    #endif
                */

                if (message.msgid == MAVLINK_MSG_ID_HEARTBEAT)
                {
    #ifndef __mobile__
                    // Start loggin on first heartbeat
                   // _startLogging();
    #endif

                    mavlink_heartbeat_t heartbeat;
                    mavlink_msg_heartbeat_decode(&message, &heartbeat);
                    Emit([&](const IMavlinkCommsEvents* ptr){ptr->VehicleHeartbeatInfo(link, message.sysid, heartbeat.mavlink_version, heartbeat.autopilot, heartbeat.type);});
                }

                // Increase receive counter
                totalReceiveCounter[mavlinkChannel]++;
                currReceiveCounter[mavlinkChannel]++;

                // Determine what the next expected sequence number is, accounting for
                // never having seen a message for this system/component pair.
                int lastSeq = lastIndex[message.sysid][message.compid];
                int expectedSeq = (lastSeq == -1) ? message.seq : (lastSeq + 1);

                // And if we didn't encounter that sequence number, record the error
                if (message.seq != expectedSeq)
                {

                    // Determine how many messages were skipped
                    int lostMessages = message.seq - expectedSeq;

                    // Out of order messages or wraparound can cause this, but we just ignore these conditions for simplicity
                    if (lostMessages < 0)
                    {
                        lostMessages = 0;
                    }

                    // And log how many were lost for all time and just this timestep
                    totalLossCounter[mavlinkChannel] += lostMessages;
                    currLossCounter[mavlinkChannel] += lostMessages;
                }

                // And update the last sequence number for this system/component pair
                lastIndex[message.sysid][message.compid] = expectedSeq;

                // Update on every 32th packet
                if ((totalReceiveCounter[mavlinkChannel] & 0x1F) == 0)
                {
                    // Calculate new loss ratio
                    // Receive loss
                    float receiveLossPercent = (double)currLossCounter[mavlinkChannel]/(double)(currReceiveCounter[mavlinkChannel]+currLossCounter[mavlinkChannel]);
                    receiveLossPercent *= 100.0f;
                    currLossCounter[mavlinkChannel] = 0;
                    currReceiveCounter[mavlinkChannel] = 0;
                    Emit([&](const IMavlinkCommsEvents* ptr){ptr->ReceiveLossPercentChanged(message.sysid, receiveLossPercent);});
                    Emit([&](const IMavlinkCommsEvents* ptr){ptr->ReceiveLossPercentChanged(message.sysid, totalLossCounter[mavlinkChannel]);});
                }

                // The packet is emitted as a whole, as it is only 255 - 261 bytes short
                // kind of inefficient, but no issue for a groundstation pc.
                // It buys as reentrancy for the whole code over all threads
                Emit([&message](const IMavlinkCommsEvents* ptr){ptr->MessageReceived(message);});
            }
        }
    }

private:

    void Emit(const std::function<void(const IMavlinkCommsEvents*)> func)
    {
        for(const IMavlinkCommsEvents* listener : m_Listners)
            func(listener);
    }

private:

    int m_systemId;

    int lastIndex[256][256];    ///< Store the last received sequence ID for each system/componenet pair
    int totalReceiveCounter[MAVLINK_COMM_NUM_BUFFERS];    ///< The total number of successfully received messages
    int totalLossCounter[MAVLINK_COMM_NUM_BUFFERS];       ///< Total messages lost during transmission.
    int totalErrorCounter[MAVLINK_COMM_NUM_BUFFERS];      ///< Total count of all parsing errors. Generally <= totalLossCounter.
    int currReceiveCounter[MAVLINK_COMM_NUM_BUFFERS];     ///< Received messages during this sample time window. Used for calculating loss %.
    int currLossCounter[MAVLINK_COMM_NUM_BUFFERS];        ///< Lost messages during this sample time window. Used for calculating loss %.


    std::vector<const IMavlinkCommsEvents*> m_Listners;

    std::unordered_map<const ILink*, uint8_t> m_MavlinkChannels;
};


} //END MAVLINKComms

#endif // MAVLINK_COMMS_H
