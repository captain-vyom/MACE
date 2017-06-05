#ifndef ACTION_CHANGE_SPEED_H
#define ACTION_CHANGE_SPEED_H

#include <iostream>

#include "data/speed_frame.h"

#include "data/command_item_type.h"
#include "data_generic_command_item/abstract_command_item.h"


namespace CommandItem {

class ActionChangeSpeed : public AbstractCommandItem
{
public:
    virtual Data::CommandItemType getCommandType() const;

    virtual std::string getDescription() const;

    virtual bool hasSpatialInfluence() const;

public:
    ActionChangeSpeed();
    ActionChangeSpeed(const ActionChangeSpeed &obj);
    ActionChangeSpeed(const int &systemOrigin, const int &systemTarget);

public:
    void setSpeedFrame(const Data::SpeedFrame &frame)
    {
        speedFrame = frame;
    }

    Data::SpeedFrame getSpeedFrame() const
    {
        return speedFrame;
    }

    void setDesiredSpeed(const double &speed)
    {
        desiredSpeed = speed;
    }

    double getDesiredSpeed() const
    {
        return desiredSpeed;
    }

public:
    void operator = (const ActionChangeSpeed &rhs)
    {
        AbstractCommandItem::operator =(rhs);
        this->speedFrame = rhs.speedFrame;
        this->desiredSpeed = rhs.desiredSpeed;
    }

    bool operator == (const ActionChangeSpeed &rhs) {
        if(!AbstractCommandItem::operator ==(rhs))
        {
            return false;
        }
        if(this->speedFrame != rhs.speedFrame){
            return false;
        }
        if(this->desiredSpeed != rhs.desiredSpeed){
            return false;
        }
        return true;
    }

    bool operator != (const ActionChangeSpeed &rhs) {
        return !(*this == rhs);
    }

    friend std::ostream &operator<<(std::ostream &out, const ActionChangeSpeed &obj)
    {
        out<<"Command Change Speed( Target ID: "<<obj.targetSystem<<", Generating ID: "<<obj.originatingSystem<<", Frame: "<<Data::SpeedFrameToString(obj.speedFrame)<<", Speed:"<<obj.desiredSpeed<<")";
        return out;
    }

private:
    Data::SpeedFrame speedFrame;
    double desiredSpeed;

};

} //end of namespace CommandItem
#endif // ACTION_CHANGE_SPEED_H