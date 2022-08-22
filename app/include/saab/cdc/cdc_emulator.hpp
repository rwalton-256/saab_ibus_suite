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
    CanIfaceObserver _mNodeReqObserver;
    CanIfaceObserver _mCDCCommandObserver;
    std::atomic<bool> _mShutdown;
    std::atomic<bool> _mCDCActive;
    std::condition_variable _mStatusCV;
    std::mutex _mMutex;
    std::thread _mStatusThread;
    std::thread _mNodeResponseThread;
    std::thread _mCDCCommandThread;
    void _mStatusFunc();
    void _mNodeResponseFunc();
    void _mCDCCommandFunc();
public:
    CDCEmulator( CanIfaceManager& _aIfaceManager );
    ~CDCEmulator();
};

}//cdc
}//saab
