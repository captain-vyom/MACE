#include "cartesian_2D_space.h"

namespace mace {
namespace state_space {

void Cartesian2DSpace_Sampler::sampleUniform(State *state)
{
    //first grab the bounds at which we are allowed to sample within
    const Cartesian2DSpaceBounds &bounds = static_cast<const Cartesian2DSpace*>(m_space)->getBounds();

    //lets cast the state pointer into the type we are expecting it to be
    pose::CartesianPosition_2D* cast = static_cast<pose::CartesianPosition_2D*>(state);
    cast->setXPosition(m_rng.uniformReal(bounds.getMinX(),bounds.getMaxX()));
    cast->setYPosition(m_rng.uniformReal(bounds.getMinY(),bounds.getMaxY()));
}

void Cartesian2DSpace_Sampler::sampleUniformNear(State *sample, const State *near, const double distance)
{

}

void Cartesian2DSpace_Sampler::sampleGaussian(State *sample, const State *mean, const double stdDev)
{

}

} //end of namespace state_space
} //end of namespace mace
