#include "spatial_home.h"

namespace MissionItem {

MissionItemType SpatialHome::getMissionType() const
{
    return MissionItemType::RTL;
}

std::string SpatialHome::getDescription() const
{
    return "This stores the home location for a vehicle";
}

bool SpatialHome::hasSpatialMissionInfluence() const
{
    return true;
}

SpatialHome::SpatialHome()
{
    m_CoordinateFrame = Data::CoordinateFrame::NED;
    m_PositionalFrame = Data::PositionalFrame::GLOBAL;
}

SpatialHome::SpatialHome(const SpatialHome &spatialHome)
{
    this->operator =(spatialHome);
}
}
