/**
 * @file can_iface_manager.cpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <saab/can_iface_manager.hpp>

#include <algorithm>

namespace saab
{

CanIfaceManager::CanIfaceManager( const std::string& _aIfaceName, std::chrono::duration<double> _aTimeout )
    :
    _mWrapper( _aIfaceName, _aTimeout ),
    _mDestruct( false ),
    _mReceiveThread( [&](){ _mReceiveFunc(); } )
{

}

CanIfaceManager::~CanIfaceManager()
{
    _mDestruct.store( true );
    _mReceiveThread.join();
}

ssize_t CanIfaceManager::send_frame( const can_frame& _aFrame )
{
    return _mWrapper.send_frame( _aFrame );
}

void CanIfaceManager::add_observer( CanIfaceObserver* obs )
{
    std::lock_guard<std::mutex> lg( _mMutex );
    _mObservers.push_back( obs );
}

void CanIfaceManager::remove_observer( CanIfaceObserver* obs )
{
    std::lock_guard<std::mutex> lg( _mMutex );
    _mObservers.remove( obs );
}

void CanIfaceManager::_mReceiveFunc()
{
    while( !_mDestruct )
    {
        try
        {
            can_frame f;
            _mWrapper.receive_frame( f );
            std::lock_guard<std::mutex> lg( _mMutex );
            std::for_each(
                _mObservers.begin(),
                _mObservers.end(),
                [&]( CanIfaceObserver* obs )
                {
                    if( obs->get_can_id() == f.can_id )
                    {
                        obs->notify( f );
                    }
                }
            );
        }
        catch(const SocketCAN_Wrapper::Timeout& e) {}
    }
}

CanIfaceObserver::CanIfaceObserver( CanIfaceManager& _aManager, canid_t _aCanId )
    :
    _mManager( _aManager ),
    _mCanId( _aCanId )
{
    _mManager.add_observer( this );
}

CanIfaceObserver::~CanIfaceObserver()
{
    _mManager.remove_observer( this );
}

void CanIfaceObserver::notify( const can_frame& f )
{
    std::lock_guard<std::mutex> lg( _mMutex );
    _mFrame = f;
    _mCondVar.notify_all();
}

void CanIfaceObserver::wait()
{
    std::unique_lock<std::mutex> ul( _mMutex );
    _mCondVar.wait( ul );
}

}//saab
