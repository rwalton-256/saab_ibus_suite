/**
 * @file saab_status_impl.cpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <saab/status/saab_tracker.hpp>
#include <saab/status/steering_wheel_buttons.hpp>
#include <saab/status/sid_buttons.hpp>

#include <iostream>
#include <algorithm>

#include <socketcan_wrapper/can_debug.hpp>

namespace saab
{
namespace status
{

StatusFlagObserver::StatusFlagObserver( StatusFlagTracker& _aStatusFlagTracker, FlagState _aStateType )
    :
    _mStatusFlagTracker( _aStatusFlagTracker ),
    _mStateType( _aStateType )
{
    _mStatusFlagTracker.add_observer( this );
}

StatusFlagObserver::~StatusFlagObserver()
{
    _mStatusFlagTracker.remove_observer( this );
}

void StatusFlagObserver::notify()
{
    std::lock_guard<std::mutex> lg( _mMutex );
    _mCondVar.notify_all();
}

void StatusFlagObserver::wait()
{
    std::unique_lock<std::mutex> ul( _mMutex );
    _mCondVar.wait( ul );
}

void StatusFlagTracker::update( const bool& status )
{
    bool changed = ( status != _mStatus );
    StatusTracker<bool>::update(status);
    if( changed )
    {
        std::for_each(
            _mObservers.begin(),
            _mObservers.end(),
            [&](StatusFlagObserver* obs)
            {
                switch( obs->get_state_type() )
                {
                case FlagState_Changed:
                    obs->notify();
                    break;
                case FlagState_Rising:
                    if( status ) obs->notify();
                    break;
                case FlagState_Falling:
                    if( !status ) obs->notify();
                    break;
                }
            }
        );
    }
}

void StatusFlagTracker::add_observer( StatusFlagObserver* obs )
{
    std::lock_guard<std::mutex> lg( _mMutex );
    _mObservers.push_back( obs );
}

void StatusFlagTracker::remove_observer( StatusFlagObserver* obs )
{
    std::lock_guard<std::mutex> lg( _mMutex );
    _mObservers.remove( obs );
}

SaabTracker::SaabTracker( CanIfaceManager& _aCanIfaceManager )
    :
    _mSIDButtons( _aCanIfaceManager ),
    _mSteeringWheelButtons( _aCanIfaceManager )
{

}

SteeringWheelButtonsTracker::SteeringWheelButtonsTracker( CanIfaceManager& _aCanIfaceManager )
    : StatusTrackerManager( _aCanIfaceManager )
{

}

void SteeringWheelButtonsTracker::update( const can_frame& f )
{
//    std::cout << "Update Steering Wheel Buttons" << std::endl << f;
    _mVolMinus.update( f.data[4] & 0x80 );
    _mVolPlus.update( f.data[4] & 0x40 );
    _mSrc.update( f.data[4] & 0x20 );
    _mSeekPlus.update( f.data[4] & 0x10 );
    _mSeekMinus.update( f.data[4] & 0x08 );
    _mNext.update( f.data[4] & 0x04 );
}

SIDButtonsTracker::SIDButtonsTracker( CanIfaceManager& _aCanIfaceManager )
    : StatusTrackerManager( _aCanIfaceManager )
{

}

void SIDButtonsTracker::update( const can_frame& f )
{
//    std::cout << "Update SID Buttons" << std::endl << f;
    _mClr.update( f.data[5] & 0x80 );
    _mSet.update( f.data[5] & 0x40 );
    _mDown.update( f.data[5] & 0x20 );
    _mUp.update( f.data[5] & 0x10 );
    _mNpanel.update( f.data[5] & 0x08 );
    _mClkPlus.update( f.data[5] & 0x04 );
    _mClkMinus.update( f.data[5] & 0x02 );
}

}//status
}//saab

std::ostream& operator<<( std::ostream& os, const saab::status::SIDButtons& sid_buttons )
{
    os << "SID Buttons Status:" << std::endl;
    os << "  clear " << sid_buttons.clear()() << std::endl;
    os << "  set " << sid_buttons.set()() << std::endl;
    os << "  down " << sid_buttons.down()() << std::endl;
    os << "  up " << sid_buttons.up()() << std::endl;
    os << "  night panel " << sid_buttons.night_panel()() << std::endl;
    os << "  clk plus " << sid_buttons.clk_plus()() << std::endl;
    os << "  clk minus " << sid_buttons.clk_minus()() << std::endl;
    return os;
}
