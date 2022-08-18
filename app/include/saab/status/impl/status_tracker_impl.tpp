/**
 * @file status_tracker_impl.tpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <saab/status/status_tracker.hpp>

namespace saab
{
namespace status
{

template<typename StatusType>
StatusTracker<StatusType>::StatusTracker()
    :
    _mStatusTimestamp( std::chrono::time_point<std::chrono::system_clock>::min() )
{

}

template<typename StatusType>
void StatusTracker<StatusType>::update( const StatusType& status )
{
    _mStatusTimestamp = std::chrono::high_resolution_clock::now();
    _mStatus = status;
}

template<typename StatusType>
const StatusType& StatusTracker<StatusType>::raw() const
{
    return _mStatus;
}

template<typename StatusType>
const bool StatusTracker<StatusType>::valid() const
{
    return true;
}

template<canid_t id>
StatusTrackerManager<id>::StatusTrackerManager( CanIfaceManager& _aCanIfaceManager )
    :
    _mObserver( _aCanIfaceManager, id ),
    _mShutdown( false ),
    _mThread( [&](){ _mFunc(); } )
{

}

template<canid_t id>
void StatusTrackerManager<id>::_mFunc()
{
    while( !_mShutdown )
    {
        _mObserver.wait();
        if( !_mShutdown )
        {
            update( _mObserver() );
        }
    }
}

template<canid_t id>
StatusTrackerManager<id>::~StatusTrackerManager()
{
    _mShutdown = true;
    _mObserver.notify( can_frame() );
    _mThread.join();
}

}//status
}//saab
