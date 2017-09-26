#ifndef STATE_SPACE_H
#define STATE_SPACE_H

#include <vector>
#include <string>

#include "base/base_global.h"

#include "state_space_types.h"
#include "state.h"

namespace mace {
namespace state_space {

class BASESHARED_EXPORT StateSpace
{
public:
    StateSpace();

    virtual ~StateSpace();

    StateSpace(const StateSpace& copy) = delete;
    StateSpace &operator =(const StateSpace &copy) = delete;

    typedef State StateType;

    template <class T>
    T *as()
    {
        return static_cast<T*>(this);
    }

    template <class T>
    const T *as() const
    {
        return static_cast<const T*>(this);
    }

public:
    const std::string &getName() const
    {
        return m_name;
    }

    void setName(const std::string &name)
    {
        this->m_name = name;
    }

    StateSpaceTypes getType() const
    {
        return m_type;
    }

public:
    virtual double distanceBetween(const State* lhs, const State* rhs) const = 0;

    /** The purpose of the following functions is to handle the state definition
        from the state space is that sampled. We allow for the space to handle
        the memory allocation in the event there are specifics unaware to the
        class implementors. It should be that this class shall handle the deletion
        of the state.
    */
public:
    //!
    //! \brief getNewState
    //! \return
    //!
    virtual State* getNewState() const = 0;

    //!
    //! \brief removeState
    //! \param state
    //!
    virtual void removeState(State* state) const = 0;

    //!
    //! \brief removeStates
    //! \param states
    //!
    virtual void removeStates(std::vector<State*> states) const = 0;

protected:
    StateSpaceTypes m_type;

    /** Members to handle validation of the state/state transition. */
protected:

private:
    std::string m_name;

};

} //end of namespace state_space
} //end of namespace mace

#endif // STATE_SPACE_H
