/**
 * @file steering_wheel_buttons.hpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <saab/status/status_tracker.hpp>
#include <saab/can_iface_manager.hpp>

namespace saab
{
namespace status
{

struct SteeringWheelButtons
{
public:
    StatusFlagTracker _mVolMinus;
    StatusFlagTracker _mVolPlus;
    StatusFlagTracker _mSrc;
    StatusFlagTracker _mSeekMinus;
    StatusFlagTracker _mSeekPlus;
    StatusFlagTracker _mNext;
};

class SteeringWheelButtonsTracker : public SteeringWheelButtons, public StatusTrackerManager<0x290>
{
public:
    SteeringWheelButtonsTracker( CanIfaceManager& _aCanIfaceManager );
    void update( const can_frame& f );
};

}//current
}//saab
