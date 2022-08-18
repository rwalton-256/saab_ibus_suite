#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <csignal>

#include <saab/can_iface_manager.hpp>
#include <saab/status/saab_tracker.hpp>
#include <saab/cdc/cdc_emulator.hpp>

#include <socketcan_wrapper/can_debug.hpp>

std::condition_variable cv;

int main( int argc, char** argv )
{
    saab::CanIfaceManager can_iface_manager( "vcan0" );

    saab::status::SaabTracker saab_tracker( can_iface_manager );

    saab::status::StatusFlagObserver clr_rise_observer( saab_tracker.sid_buttons().clear(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver clr_fall_observer( saab_tracker.sid_buttons().clear(), saab::status::FlagState_Falling );
    saab::status::StatusFlagObserver clk_minus_rise_observer( saab_tracker.sid_buttons().clk_minus(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver clk_minus_fall_observer( saab_tracker.sid_buttons().clk_minus(), saab::status::FlagState_Falling );
    saab::status::StatusFlagObserver clk_plus_rise_observer( saab_tracker.sid_buttons().clk_plus(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver clk_plus_fall_observer( saab_tracker.sid_buttons().clk_plus(), saab::status::FlagState_Falling );
    saab::status::StatusFlagObserver down_rise_observer( saab_tracker.sid_buttons().down(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver down_fall_observer( saab_tracker.sid_buttons().down(), saab::status::FlagState_Falling );
    saab::status::StatusFlagObserver night_rise_observer( saab_tracker.sid_buttons().night_panel(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver night_fall_observer( saab_tracker.sid_buttons().night_panel(), saab::status::FlagState_Falling );
    saab::status::StatusFlagObserver set_rise_observer( saab_tracker.sid_buttons().set(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver set_fall_observer( saab_tracker.sid_buttons().set(), saab::status::FlagState_Falling );
    saab::status::StatusFlagObserver up_rise_observer( saab_tracker.sid_buttons().up(), saab::status::FlagState_Rising );
    saab::status::StatusFlagObserver up_fall_observer( saab_tracker.sid_buttons().up(), saab::status::FlagState_Falling );

    std::thread clr_rise_thread( [&]()
    {
        while(1)
        {
            clr_rise_observer.wait();
            std::cout << "Clear rising edge!" << std::endl;
        }
    } );
    std::thread clr_fall_thread( [&]()
    {
        while(1)
        {
            clr_fall_observer.wait();
            std::cout << "Clear falling edge!" << std::endl;
        }
    } );
    std::thread clk_minus_rise_thread( [&]()
    {
        while(1)
        {
            clk_minus_rise_observer.wait();
            std::cout << "Clk Minus rising edge!" << std::endl;
        }
    } );
    std::thread clk_minus_fall_thread( [&]()
    {
        while(1)
        {
            clk_minus_fall_observer.wait();
            std::cout << "Clk Minus falling edge!" << std::endl;
        }
    } );
    std::thread clk_plus_rise_thread( [&]()
    {
        while(1)
        {
            clk_plus_rise_observer.wait();
            std::cout << "Clk Plus rising edge!" << std::endl;
        }
    } );
    std::thread clk_plus_fall_thread( [&]()
    {
        while(1)
        {
            clk_plus_fall_observer.wait();
            std::cout << "Clk Plus falling edge!" << std::endl;
        }
    } );
    std::thread down_rise_thread( [&]()
    {
        while(1)
        {
            down_rise_observer.wait();
            std::cout << "Down rising edge!" << std::endl;
        }
    } );
    std::thread down_fall_thread( [&]()
    {
        while(1)
        {
            down_fall_observer.wait();
            std::cout << "Down falling edge!" << std::endl;
        }
    } );
    std::thread night_rise_thread( [&]()
    {
        while(1)
        {
            night_rise_observer.wait();
            std::cout << "Night rising edge!" << std::endl;
        }
    } );
    std::thread night_fall_thread( [&]()
    {
        while(1)
        {
            night_fall_observer.wait();
            std::cout << "Night falling edge!" << std::endl;
        }
    } );
    std::thread set_rise_thread( [&]()
    {
        while(1)
        {
            set_rise_observer.wait();
            std::cout << "Set rising edge!" << std::endl;
        }
    } );
    std::thread set_fall_thread( [&]()
    {
        while(1)
        {
            set_fall_observer.wait();
            std::cout << "Set falling edge!" << std::endl;
        }
    } );
    std::thread up_rise_thread( [&]()
    {
        while(1)
        {
            up_rise_observer.wait();
            std::cout << "Up rising edge!" << std::endl;
        }
    } );
    std::thread up_fall_thread( [&]()
    {
        while(1)
        {
            up_fall_observer.wait();
            std::cout << "Up falling edge!" << std::endl;
        }
    } );

    saab::cdc::CDCEmulator cdc_emulator( can_iface_manager );

    signal( SIGINT, []( int signal ){ cv.notify_one(); } );

    std::mutex m;
    std::unique_lock<std::mutex> ul( m );
    cv.wait( ul );

    return 0;
}
