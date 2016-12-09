#ifndef ARDUPILOTATTITUDE_H
#define ARDUPILOTATTITUDE_H

#include <Eigen/Dense>
#include <mavlink.h>

namespace Ardupilot{

class ArdupilotAttitude
{
public:
    ArdupilotAttitude();
    void updateAttitudeMavlink(const mavlink_attitude_t &msgData);
    void getAttitude(Eigen::Vector3d &attitudeVector);

private:
    double roll;
    double roll_rate;
    double pitch;
    double pitch_rate;
    double yaw;
    double yaw_rate;

};
} //end of namespace Ardupilot

#endif // ARDUPILOTATTITUDE_H