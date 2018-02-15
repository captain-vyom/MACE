#ifndef ARDUPILOT_STATE_TYPES_H
#define ARDUPILOT_STATE_TYPES_H

#include <string>
#include <vector>
#include <stdexcept>


namespace ardupilot{
namespace state {

enum class ArdupilotFlightState{
    STATE_GROUNDED = 0, /**< */
    STATE_GROUNDED_IDLE = 1, /**< */
    STATE_GROUNDED_ARMING = 2, /**< */
    STATE_GROUNDED_ARMED = 3, /**< */
    STATE_GROUNDED_DISARMING = 4, /**< */
    STATE_FLIGHT = 5, /**< */
    STATE_TAKEOFF = 6, /**< */
    STATE_TAKEOFF_CLIMBING = 7, /**< */
    STATE_TAKEOFF_TRANSITIONING = 8, /**< */
    STATE_FLIGHT_MANUAL = 9, /**< */
    STATE_FLIGHT_GUIDED = 10, /**< */
    STATE_FLIGHT_AUTO = 11, /**< */
    STATE_FLIGHT_BRAKE= 12, /**< */
    STATE_LANDING= 13, /**< */
    STATE_LANDING_TRANSITIONING = 14, /**< */
    STATE_LANDING_DESCENDING = 15 /**< */
};

//!
//! \brief CommandToString
//! \param type
//! \return
//!
inline std::string ArdupilotStateToString(const ArdupilotFlightState &type) {
    switch (type) {
    case ArdupilotFlightState::STATE_GROUNDED:
        return "Grounded";
    case ArdupilotFlightState::STATE_GROUNDED_IDLE:
        return "Grounded Idle";
    case ArdupilotFlightState::STATE_GROUNDED_ARMING:
        return "Grounded Arming";
    case ArdupilotFlightState::STATE_GROUNDED_ARMED:
        return "Grounded Armed";
    case ArdupilotFlightState::STATE_GROUNDED_DISARMING:
        return "Grounded Disarming";
    case ArdupilotFlightState::STATE_FLIGHT:
        return "Flight";
    case ArdupilotFlightState::STATE_TAKEOFF:
        return "Flight Takeoff";
    case ArdupilotFlightState::STATE_TAKEOFF_CLIMBING:
        return "Flight Takeoff Climbing";
    case ArdupilotFlightState::STATE_TAKEOFF_TRANSITIONING:
        return "Flight Takeoff Transitioning";
    case ArdupilotFlightState::STATE_FLIGHT_MANUAL:
        return "Flight Manual";
    case ArdupilotFlightState::STATE_FLIGHT_GUIDED:
        return "Flight Guided";
    case ArdupilotFlightState::STATE_FLIGHT_AUTO:
        return "Flight Auto";
    case ArdupilotFlightState::STATE_FLIGHT_BRAKE:
        return "Flight Brake";
    case ArdupilotFlightState::STATE_LANDING:
        return "Flight Landing";
    case ArdupilotFlightState::STATE_LANDING_TRANSITIONING:
        return "Flight Landing Transitioning";
    case ArdupilotFlightState::STATE_LANDING_DESCENDING:
        return "Flight Landing Descent";
    default:
        throw std::runtime_error("Unknown ardupilot state type seen");
    }
}

} //end of namespace ardupilot
} //end of namespace state


#endif // ARDUPILOT_STATE_TYPES_H
