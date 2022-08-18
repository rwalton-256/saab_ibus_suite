/**
 * @file can_iface_manager.hpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <list>
#include <mutex>
#include <condition_variable>

#include <socketcan_wrapper/socketcan_wrapper.hpp>

namespace saab
{

class CanIfaceObserver;

class CanIfaceManager
{
    typedef const can_frame& cb_arg_type;
    typedef std::function<void(cb_arg_type)> cb_type;

    SocketCAN_Wrapper _mWrapper;
    std::mutex _mMutex;
    std::list<CanIfaceObserver*> _mObservers;
    std::atomic<bool> _mDestruct;
    std::thread _mReceiveThread;
public:
    CanIfaceManager( const std::string& _aIfaceName, std::chrono::duration<double> _aTimeout = std::chrono::milliseconds( 20 ) );
    ~CanIfaceManager();

    ssize_t send_frame( const can_frame& _aFrame );
    void add_observer( CanIfaceObserver* obs );
    void remove_observer( CanIfaceObserver* obs );
private:
    void _mReceiveFunc();
};

class CanIfaceObserver
{
private:
    CanIfaceManager& _mManager;
    can_frame _mFrame;
    canid_t _mCanId;
    std::mutex _mMutex;
    std::condition_variable _mCondVar;
public:
    CanIfaceObserver( CanIfaceManager& _aManager, canid_t _aCanId );
    ~CanIfaceObserver();
    canid_t get_can_id() const { return _mCanId; }
    void notify( const can_frame& f );
    void wait();
    const can_frame& operator()() const { return _mFrame; }
};

}//saab
