/**
 * @file cdc_emulator.hpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <saab/can_iface_manager.hpp>

namespace saab
{
namespace cdc
{

class CDCEmulator
{
protected:
    CanIfaceManager& _mIfaceManager;
    std::thread _mStatusThread;
    std::thread _mNodeResponseThread;
    std::atomic<bool> _mShutdown;
    CanIfaceObserver _mNodeReqObserver;
    std::atomic<bool> _mActive;
    void _mStatusFunc();
    void _mNodeResponseFunc();
public:
    CDCEmulator( CanIfaceManager& _aIfaceManager );
    ~CDCEmulator();
};

}//cdc
}//saab
