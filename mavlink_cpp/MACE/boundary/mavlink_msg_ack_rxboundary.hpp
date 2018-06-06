// MESSAGE ACK_RXBOUNDARY support class

#pragma once

namespace mavlink {
namespace boundary {
namespace msg {

/**
 * @brief ACK_RXBOUNDARY message
 *
 * This message is emitted after a mace has finished receiving a boundary from another MACE instance. Seeing this response should cause MACE core to update the boundary as appropriate and notify other modules of the change based on the boundary type received.
 */
struct ACK_RXBOUNDARY : mavlink::Message {
    static constexpr msgid_t MSG_ID = 131;
    static constexpr size_t LENGTH = 3;
    static constexpr size_t MIN_LENGTH = 3;
    static constexpr uint8_t CRC_EXTRA = 36;
    static constexpr auto NAME = "ACK_RXBOUNDARY";


    uint8_t boundary_system; /*< System ID */
    uint8_t boundary_creator; /*< Creator ID */
    uint8_t boundary_type; /*< Boundary type, see BOUNDARY_TYPE */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  boundary_system: " << +boundary_system << std::endl;
        ss << "  boundary_creator: " << +boundary_creator << std::endl;
        ss << "  boundary_type: " << +boundary_type << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << boundary_system;               // offset: 0
        map << boundary_creator;              // offset: 1
        map << boundary_type;                 // offset: 2
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> boundary_system;               // offset: 0
        map >> boundary_creator;              // offset: 1
        map >> boundary_type;                 // offset: 2
    }
};

} // namespace msg
} // namespace boundary
} // namespace mavlink
