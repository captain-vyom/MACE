#ifndef RRT_BASE_H
#define RRT_BASE_H

#include <memory>

#include "planners.h"
#include "rrt_node.h"
#include "nearest_neighbor_abstract.h"

#include "base/math/random_number_generator.h"

#include "base/state_space/goal_state.h"
#include "base/state_space/state_sampler.h"
#include "base/state_space/space_information.h"

namespace mace {
namespace planners_sampling{

class RRTBase : public planners::Planners{

public:
    RRTBase(const state_space::SpaceInformationPtr &spaceInfo):
        Planners(spaceInfo), goalProbability(0.1), maxBranchLength(1.0)
    {
        m_samplingStrategy = spaceInfo->getStateSampler();
    }

public:
    void setPlanningParameters(state_space::GoalState *begin, state_space::GoalState *end) override;

    std::vector<state_space::State*> solve() override;

public:

    template <class T>
    void setNearestNeighbor()
    {
        m_nnStrategy = std::make_shared<T>();
        m_nnStrategy->setDistanceFunction([this](const RootNode* lhs, const RootNode *rhs)
        {
            return distanceFunction(lhs,rhs);
        });
    }

    /** The basis for the following functions is that these are base paramters
        required for any tree based search. */
public:
    void setGoalProbability(const double &probability);

    double getGoalProbability() const;

    void setMaxBranchLength(const double &length);

    double getMaxBranchLength() const;

private:
    double distanceFunction(const RootNode* lhs, const RootNode* rhs)
    {
        return m_spaceInfo->distanceBetween(lhs->getCurrentState(), rhs->getCurrentState());
    }

protected:

    state_space::StateSamplerPtr m_samplingStrategy;

    //!
    //! \brief m_RNG
    //!
    math::RandomNumberGenerator m_RNG;

    std::shared_ptr<nn::NearestNeighborAbstract<RootNode*>> m_nnStrategy;
    /**
     * @brief The probability of biasing samples towards the goal to ensure the tree
     * indeed expands and tries to connect to the goal rather than randomly through
     * the space.
     */
    double goalProbability = 0.1;

    /**
     * @brief The maximum branch length used to connect from a root of the tree to
     * the random sampled point. This length will determine where a new root of the
     * tree is added between the two points.
     */
    double maxBranchLength = 1.0;
};


} //end of namespace planners_sampling
} //end of namespace mace


#endif // RRT_BASE_H