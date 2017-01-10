#include "local_velocity.h"

#include <math.h>

namespace DataVehicleGeneric
{


MaceCore::TopicComponentStructure LocalVelocity::TopicStructure() {
    MaceCore::TopicComponentStructure structure;
    structure.AddTerminal<double>("x");
    structure.AddTerminal<double>("y");
    structure.AddTerminal<double>("z");
    structure.AddTerminal<CoordinateFrame>("frame");

    return structure;
}


std::string LocalVelocity::Name(){
    return "local_velocity";
}


MaceCore::TopicDatagram LocalVelocity::GenerateDatagram() const {
    MaceCore::TopicDatagram datagram;
    datagram.AddTerminal<double>("x", x);
    datagram.AddTerminal<double>("y", y);
    datagram.AddTerminal<double>("z", z);
    datagram.AddTerminal<CoordinateFrame>("frame", frame);

    return datagram;
}


void LocalVelocity::CreateFromDatagram(const MaceCore::TopicDatagram &datagram) {
    x = datagram.GetTerminal<double>("x");
    y = datagram.GetTerminal<double>("y");
    z = datagram.GetTerminal<double>("z");
    frame = datagram.GetTerminal<CoordinateFrame>("frame");
}

}
