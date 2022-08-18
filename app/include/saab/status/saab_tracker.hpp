/**
 * @file saab_tracker.hpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <saab/status/sid_buttons.hpp>
#include <saab/status/steering_wheel_buttons.hpp>

namespace saab
{
namespace status
{

class SaabTracker
{
    SIDButtonsTracker _mSIDButtons;
    SteeringWheelButtonsTracker _mSteeringWheelButtons;
public:
    SaabTracker( CanIfaceManager& _aCanIfaceManager );
    const SIDButtonsTracker& sid_buttons() const { return _mSIDButtons; }
    const SteeringWheelButtons& steering_wheel_buttons() const { return _mSteeringWheelButtons; }
    SIDButtonsTracker& sid_buttons() { return _mSIDButtons; }
    SteeringWheelButtons& steering_wheel_buttons() { return _mSteeringWheelButtons; }
};

}//current
}//saab
