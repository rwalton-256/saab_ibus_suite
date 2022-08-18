/**
 * @file sid_buttons.hpp
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

#include <iostream>

namespace saab
{
namespace status
{

struct SIDButtons
{
protected:
    StatusFlagTracker _mClr;
    StatusFlagTracker _mSet;
    StatusFlagTracker _mDown;
    StatusFlagTracker _mUp;
    StatusFlagTracker _mNpanel;
    StatusFlagTracker _mClkPlus;
    StatusFlagTracker _mClkMinus;
public:
    const StatusFlagTracker& clear() const { return _mClr; }
    const StatusFlagTracker& set() const { return _mSet; }
    const StatusFlagTracker& down() const { return _mDown; }
    const StatusFlagTracker& up() const { return _mUp; }
    const StatusFlagTracker& night_panel() const { return _mNpanel; }
    const StatusFlagTracker& clk_plus() const { return _mClkPlus; }
    const StatusFlagTracker& clk_minus() const { return _mClkMinus; }
    StatusFlagTracker& clear() { return _mClr; }
    StatusFlagTracker& set() { return _mSet; }
    StatusFlagTracker& down() { return _mDown; }
    StatusFlagTracker& up() { return _mUp; }
    StatusFlagTracker& night_panel() { return _mNpanel; }
    StatusFlagTracker& clk_plus() { return _mClkPlus; }
    StatusFlagTracker& clk_minus() { return _mClkMinus; }
};

class SIDButtonsTracker : public SIDButtons, public StatusTrackerManager<0x290>
{
public:
    SIDButtonsTracker( CanIfaceManager& _aCanIfaceManager );
    void update( const can_frame& f );

};

}//current
}//saab

std::ostream& operator<<( std::ostream&, const saab::status::SIDButtons& );
