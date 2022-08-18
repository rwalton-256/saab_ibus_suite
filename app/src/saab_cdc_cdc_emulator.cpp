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
    _mStatusThread( [&](){ _mStatusFunc(); } ),
    _mNodeResponseThread( [&](){ _mNodeResponseFunc(); } ),
    _mNodeReqObserver( _mIfaceManager, 0x6A1 )
{

}

CDCEmulator::~CDCEmulator()
{
    _mShutdown = true;
    _mNodeReqObserver.notify( can_frame() );
    _mStatusThread.join();
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
                std::cout << "Sending power on resp" << std::endl;
                _mIfaceManager.send_frame( power_on_resp[0] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( power_on_resp[1] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( power_on_resp[2] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( power_on_resp[3] );
                break;
            case 0x02:
                std::cout << "Sending active resp" << std::endl;
                _mIfaceManager.send_frame( active_resp[0] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( active_resp[1] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( active_resp[2] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( active_resp[3] );
                break;
            case 0x08:
                std::cout << "Sending power down resp" << std::endl;
                _mIfaceManager.send_frame( power_down_resp[0] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( power_down_resp[1] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( power_down_resp[2] );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                _mIfaceManager.send_frame( power_down_resp[3] );
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
        .data = { 0x40, 0xff, 0x3f, 0x41, 0xff, 0xff, 0xff, 0xD0 }
    };
    while( !_mShutdown )
    {
        _mIfaceManager.send_frame( f );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    }
}

}//cdc
}//saab
