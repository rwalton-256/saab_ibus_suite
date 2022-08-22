/**
 * @file saab_cdc_cdc_emulator.cpp
 * @author Ryan Walton
 * @brief 
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <saab/cdc/cdc_emulator.hpp>

#include <cstring>
#include <iostream>

namespace saab
{
namespace cdc
{

CDCEmulator::CDCEmulator( CanIfaceManager& _aIfaceManager )
    :
    _mIfaceManager( _aIfaceManager ),
    _mShutdown( false ),
    _mCDCActive( false ),
    _mNodeReqObserver( _mIfaceManager, 0x6A1 ),
    _mCDCCommandObserver( _mIfaceManager, 0x3C0 ),
    _mStatusThread( [&](){ _mStatusFunc(); } ),
    _mNodeResponseThread( [&](){ _mNodeResponseFunc(); } ),
    _mCDCCommandThread( [&](){ _mCDCCommandFunc(); } )
{

}

CDCEmulator::~CDCEmulator()
{
    _mShutdown = true;
    _mNodeReqObserver.notify( can_frame() );
    _mCDCCommandObserver.notify( can_frame() );
    _mStatusThread.join();
    _mNodeResponseThread.join();
    _mCDCCommandThread.join();
}

void CDCEmulator::_mNodeResponseFunc()
{
    can_frame power_on_resp[4] =
    {
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x32, 0x00, 0x00, 0x03, 0x01, 0x02, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x42, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x52, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x62, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00 }
        }
    };
    can_frame active_resp[4] =
    {
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x32, 0x00, 0x00, 0x16, 0x01, 0x02, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x42, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x52, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x62, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00 }
        }
    };
    can_frame power_down_resp[4] =
    {
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x32, 0x00, 0x00, 0x19, 0x01, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x42, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x52, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00, 0x00 }
        },
        {
            .can_id = 0x6A2,
            .can_dlc = 0x08,
            .data = { 0x62, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00, 0x00 }
        }
    };
    while( !_mShutdown )
    {
        _mNodeReqObserver.wait();
        if( !_mShutdown )
        {
            switch( _mNodeReqObserver().data[3] & 0x0f )
            {
            case 0x03:
                _mIfaceManager.send_frame( power_on_resp[0] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( power_on_resp[1] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( power_on_resp[2] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( power_on_resp[3] );
                break;
            case 0x02:
                _mIfaceManager.send_frame( active_resp[0] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( active_resp[1] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( active_resp[2] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( active_resp[3] );
                break;
            case 0x08:
                _mIfaceManager.send_frame( power_down_resp[0] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( power_down_resp[1] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( power_down_resp[2] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                _mIfaceManager.send_frame( power_down_resp[3] );
                break;
            }
        }
    }
}

void CDCEmulator::_mCDCCommandFunc()
{
    while( !_mShutdown )
    {
        _mCDCCommandObserver.wait();
        if( _mShutdown ) break;
        const can_frame& f = _mCDCCommandObserver();
        if( f.data[0] == 0x80 )
        {
            switch( f.data[1] )
            {
            case 0x24:
                // CDC on
                std::cout << "CDC On" << std::endl;
                _mCDCActive.store( true );
                _mStatusCV.notify_one();
                break;
            case 0x14:
                // CDC off
                std::cout << "CDC Off" << std::endl;
                _mCDCActive.store( false );
                _mStatusCV.notify_one();
                break;
            case 0x59:
                // NXT
                std::cout << "Next" << std::endl;
                break;
            case 0x35:
                // Seek +
                std::cout << "Seek +" << std::endl;
                break;
            case 0x36:
                // Seek -
                std::cout << "Seek -" << std::endl;
                break;
            case 0x68:
                // CD selection:
                std::cout << "CD Select: " << +f.data[2] << std::endl;
                break;
            default:
                break;
            }
        }
    }
}

void CDCEmulator::_mStatusFunc()
{
    can_frame f =
    {
        .can_id = 0x3C8,
        .can_dlc = 0x8,
    };
    while( !_mShutdown )
    {
        std::unique_lock<std::mutex> ul( _mMutex );
        std::cv_status timeout = _mStatusCV.wait_for( ul, std::chrono::seconds( 1 ) );
        f.data[0] = ( timeout == std::cv_status::timeout ? 0x00 : 0x80 ) | 0x20;
        f.data[1] = _mCDCActive.load() ? 0xff : 0x00;
        f.data[2] = _mCDCActive.load() ? 0x3f : 0x01;
        f.data[3] = _mCDCActive.load() ? 0x4f : 0x01;
        f.data[4] = 0xff;
        f.data[5] = 0xff;
        f.data[6] = 0xff;
        f.data[7] = 0xd0;

        if( _mShutdown ) break;
        _mIfaceManager.send_frame( f );
    }
}

}//cdc
}//saab
