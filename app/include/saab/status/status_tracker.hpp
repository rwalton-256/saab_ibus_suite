/**
 * @file current_status.hpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <saab/can_iface_manager.hpp>

#include <chrono>
#include <mutex>
#include <list>

#include <linux/can.h>

namespace saab
{
namespace status
{

template<typename StatusType>
class StatusTracker
{
protected:
    StatusType _mStatus;
    std::chrono::time_point<std::chrono::system_clock> _mStatusTimestamp;
    std::mutex _mMutex;
public:
    StatusTracker();
    void update( const StatusType& status );
    const StatusType& raw() const;
    const bool valid() const;
    const StatusType& operator()() const { return _mStatus; }
    StatusType operator()() { return _mStatus; }
};

class StatusFlagObserver;

enum FlagState
{
    FlagState_Changed,
    FlagState_Rising,
    FlagState_Falling
};

class StatusFlagTracker : public StatusTracker<bool>
{
protected:
    std::list<StatusFlagObserver*> _mObservers;
public:
    StatusFlagTracker() { _mStatus = false; }
    void update( const bool& status );
    void add_observer( StatusFlagObserver* obs );
    void remove_observer( StatusFlagObserver* obs );
};

template<canid_t id>
class StatusTrackerManager
{
    std::thread _mThread;
    CanIfaceObserver _mObserver;
    std::atomic<bool> _mShutdown;
    void _mFunc();
public:
    StatusTrackerManager( CanIfaceManager& _aCanIfaceManager );
    ~StatusTrackerManager();
    virtual void update( const can_frame& ) = 0;
};

class StatusFlagObserver
{
private:
    std::mutex _mMutex;
    std::condition_variable _mCondVar;
    StatusFlagTracker& _mStatusFlagTracker;
    const FlagState _mStateType;
public:
    StatusFlagObserver( StatusFlagTracker& _aStatusFlagTracker, FlagState _aStateType );
    ~StatusFlagObserver();
    void wait();
    void notify();
    FlagState get_state_type() const { return _mStateType; }
    StatusFlagTracker& operator()() { return _mStatusFlagTracker; }
};

}//status
}//saab

#include <saab/status/impl/status_tracker_impl.tpp>
