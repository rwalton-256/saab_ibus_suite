/**
 * @file can_bus_protocol.hpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <linux/can.h>

namespace saab
{
namespace protocol
{

struct SID_Pedals : can_frame
{
public:
    bool get_pedal_kickdown() const;
    bool get_pedal_brake() const;
    
};

}//protocol
}//saab
