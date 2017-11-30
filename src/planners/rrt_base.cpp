#include "rrt_base.h"

namespace mace {
namespace planners_sampling{

void RRTBase::setPlanningParameters(state_space::GoalState *begin, state_space::GoalState *end)
{
    m_stateBegin = begin;
    m_stateEnd = end;
}

//!
//! \brief RRTBase::solve function that initiates the solve routine of the planner
//!
std::vector<state_space::State*> RRTBase::solve()
{

    RootNode* finalNode = nullptr;
    bool solutionFound = false;
    std::vector<state_space::State*> path;

    /**
     * 1. Create the root node of the search based on the starting state and
     * insert into the allocated tree structure.
     */
    state_space::State* startState = m_spaceInfo->copyState(m_stateBegin->getState());
    RootNode* start = new RootNode(startState);
    m_nnStrategy->add(start);

    while(true){
        RootNode* sampleNode = new RootNode(m_spaceInfo->getStateSpace());
        //get the state from the node so that we can update this in memory when sampling
        state_space::State* sampleState = sampleNode->getCurrentState();
        /**
         * 2. Sample a state from the state space
         */
        if((m_stateEnd != nullptr) && (m_RNG.uniform01() < goalProbability))
            m_stateEnd->sampleGoal(sampleState);
        else
            m_samplingStrategy->sampleUniform(sampleState);

        /**
         * 3. Find the closet other node in the tree and determine the distance to the node
         */
        RootNode* closestNode = m_nnStrategy->nearest(sampleNode);
        state_space::State* closestState = closestNode->getCurrentState();
        double distance = m_spaceInfo->distanceBetween(closestState,sampleState);

        /**
         * 4. Determine if the sampled point is within the appropriate distance threshold.
         * If not, interpolate between the states if possible. If not, drop the sample and
         * move on. If so, continue.
         */
        if(distance > maxBranchLength)
        {
            //do the interpretation
            bool validity = m_spaceInfo->getStateSpace()->interpolateStates(closestState,sampleState,maxBranchLength/distance,&sampleState);
            //Madison, why do I need to do this
            sampleNode->setCurrentState(sampleState);
        }

        /**
         * 5. Check that 1)State is valid and collision free, 2)Path edge is valid sampled at desired intervals
         * related to the aircraft size
         */
        if(m_spaceInfo->isEdgeValid(closestState,sampleState))
        {
            /**
             *5a. At this point the sampled state is clearly valid
             */
            sampleNode->setParentNode(closestNode);
            m_nnStrategy->add(sampleNode);

            /**
             *5b. Check if this satisfies the goal criteria
             */
            solutionFound = m_stateEnd->isGoalSatisfied(sampleState);
            if(solutionFound == true)
            {
                finalNode = sampleNode;
                break;
            }
        }
    } //end of while loop

    if(finalNode != nullptr)
    {
        while(finalNode != nullptr)
        {
            path.push_back(finalNode->getCurrentState());
            finalNode = finalNode->getParentNode();
        }
    }

    return path;
}

void RRTBase::setGoalProbability(const double &probability)
{
    goalProbability = probability;
}

double RRTBase::getGoalProbability() const{
    return this->goalProbability;
}

void RRTBase::setMaxBranchLength(const double &length)
{
    maxBranchLength = length;
}

double RRTBase::getMaxBranchLength() const
{
    return this->maxBranchLength;
}

} //end of namespace planners_sampling
} //end of namespace mace