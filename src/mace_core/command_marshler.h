#ifndef EVENT_LOOP_MARSHLER_H
#define EVENT_LOOP_MARSHLER_H

#include <functional>
#include <unordered_map>
#include <vector>
#include <list>
#include <mutex>
#include <memory>
#include <ctime>

#include "mace_core_global.h"


//!
//! \brief Object that stores functions to call given some enumeration.
//!
//! Can either store calls to be invoked at a later time (potentially on another thread)
//! Or provide a way to immediatly call the function.
//!
template<typename T>
class CommandMarshler
{
private:


    class FunctionCallerBase
    {
    public:
        virtual void Call() = 0;

        virtual int NumCalls() const = 0;

    protected:

        std::mutex m_Mutex;
    };


    class ZeroParamCaller : public FunctionCallerBase
    {
    public:
        ZeroParamCaller(const std::function<void()> &func) :
            m_Func(func)
        {
        }

        void Enable()
        {
            isEnabled = true;
        }

        virtual int NumCalls() const
        {
            if(isEnabled)
                return 1;
            return 0;
        }

        virtual void Call()
        {
            isEnabled = false;
            m_Func();
        }

        void InvokeOneInstance()
        {
            m_Func();
        }

    private:

        const std::function<void()> m_Func;

        bool isEnabled;
    };


    template <typename FT>
    class OneParamCaller : public FunctionCallerBase
    {
    public:
        OneParamCaller(const std::function<void(const FT&)> &func) :
            m_Func(func)
        {

        }

        void AddParameter(const FT& param)
        {
            this->m_Mutex.lock();
            m_Param1.push_back(param);
            m_Param1.unique();
            this->m_Mutex.unlock();
        }

        virtual int NumCalls() const
        {
            return m_Param1.size();
        }

        virtual void Call()
        {
            this->m_Mutex.lock();
            std::list<FT> listCpy = m_Param1;
            m_Param1.clear();
            this->m_Mutex.unlock();

            for(FT param : m_Param1)
                m_Func(param);
        }

        void InvokeOneInstance(const FT& param)
        {
            m_Func(param);
        }

    private:

        const std::function<void(const FT&)> m_Func;

        std::list<FT> m_Param1;
    };


public:


    //!
    //! \brief Add a parameterless function that gets fired when given command is issued
    //! \param event Command to triger lambda on
    //! \param lambda Lambda to trigger
    //!
    void AddLambda(T event, const std::function<void()> &lambda)
    {
        m_EventProcedures.insert({event, std::make_shared<ZeroParamCaller>(lambda)});
    }


    //!
    //! \brief Add a 1 parameter function that gets fired when given command is issued
    //! \param event Command to triger lambda on
    //! \param lambda Lambda to trigger
    //!
    template<typename P1T>
    void AddLambda(T event, const std::function<void(const P1T&)> &lambda)
    {
        m_EventProcedures.insert({event, std::make_shared<OneParamCaller<P1T>>(lambda)});
        m_CallRate.insert({event, std::chrono::milliseconds(0)});
        m_LastCallTime.insert({event, 0});
    }


    //!
    //! \brief Queue the fireing of a parameterless command
    //! \param event Command to queue the calling of internal lambda
    //!
    void QueueCommand(T event)
    {
        if(m_EventProcedures.find(event) == m_EventProcedures.cend())
            throw std::runtime_error("Given command does not have a behavior defined");

        ZeroParamCaller *caller = (ZeroParamCaller*)m_EventProcedures.at(event).get();
        caller->Enable();
    }


    //!
    //! \brief Queue the fireing of a single-parameter command
    //! \param event Command to queue the calling of internal lambda
    //!
    template<typename P1T>
    void QueueCommand(T event, const P1T &param1)
    {
        if(m_EventProcedures.find(event) == m_EventProcedures.cend())
            throw std::runtime_error("Given command does not have a behavior defined");

        OneParamCaller<P1T> *caller = (OneParamCaller<P1T>*)m_EventProcedures.at(event).get();
        caller->AddParameter(param1);
    }


    //!
    //! \brief Immediatly invoke the command on the thread this method is called from
    //! \param event Command to immediatly invoke
    //!
    void ImmediatlyCallCommand(T event)
    {
        if(m_EventProcedures.find(event) == m_EventProcedures.cend())
            throw std::runtime_error("Given command does not have a behavior defined");

        ZeroParamCaller *caller = (ZeroParamCaller*)m_EventProcedures.at(event).get();
        caller->InvokeOneInstance();
    }


    //!
    //! \brief Immediatly invoke the command on the thread this method is called from
    //! \param event Command to immediatly invoke
    //!
    template<typename P1T>
    void ImmediatlyCallCommand(T event, const P1T &param1)
    {
        if(m_EventProcedures.find(event) == m_EventProcedures.cend())
            throw std::runtime_error("Given command does not have a behavior defined");

        OneParamCaller<P1T> *caller = (OneParamCaller<P1T>*)m_EventProcedures.at(event).get();
        caller->InvokeOneInstance(param1);
    }


    //!
    //! \brief Execute lambdas for all queued commands
    //!
    void ExecuteQueuedCommands()
    {
        //make a list of events that are to be calls.
        std::vector<T> list;
        for(auto it = m_EventProcedures.cbegin() ; it != m_EventProcedures.cend() ; ++it)
        {
            T enumValue = it->first;

            std::clock_t lastTime = m_LastCallTime.at(enumValue);
            std::clock_t currTime = std::clock();

            double msDiff = (currTime - lastTime) / (double)(CLOCKS_PER_SEC / 1000.0);
            std::chrono::milliseconds rate = m_CallRate.at(enumValue);
            if(msDiff >= rate.count())
            {

                std::shared_ptr<FunctionCallerBase> caller = it->second;

                if(caller->NumCalls() > 0)
                    caller->Call();

                m_LastCallTime[enumValue] = currTime;
            }
        }
    }


    //!
    //! \brief Set the call interval of a command
    //! \param event Command to set interval of
    //! \param interval Interval to set to
    //!
    void SetCallInterval(T event, const std::chrono::milliseconds interval)
    {
        m_CallRate[event] = interval;
    }


private:



    std::unordered_map<T, std::shared_ptr<FunctionCallerBase>, EnumClassHash> m_EventProcedures;

    std::unordered_map<T, std::chrono::milliseconds, EnumClassHash> m_CallRate;

    std::unordered_map<T, std::clock_t, EnumClassHash> m_LastCallTime;
};

#endif // EVENT_LOOP_MARSHLER_H
