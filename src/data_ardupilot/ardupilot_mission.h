#ifndef ARDUPILOTMISSION_H
#define ARDUPILOTMISSION_H

namespace Ardupilot{

class ArdupilotMission
{
public:
    ArdupilotMission();

    void getCurrentMissionItem();

    void getMissionItem(const int &index);


};

} //end of namespace Ardupilot

#endif // ARDUPILOTMISSION_H