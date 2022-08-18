# CMakeLists files in this project can
cmake_minimum_required (VERSION 2.8.11)

project(sid_canutils)

add_subdirectory( lib )

find_package(Threads REQUIRED)

add_library (
    saab_instrument_buf_iface
    app/src/saab_can_iface_manager.cpp
    app/src/saab_status_impl.cpp
    app/src/saab_cdc_cdc_emulator.cpp
    )

target_include_directories ( saab_instrument_buf_iface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/app/include )

target_link_libraries( saab_instrument_buf_iface socketcan_wrapper "${CMAKE_THREAD_LIBS_INIT}" )

add_executable(
    saab_instrument_buf_iface_test
    test/test.cpp
)

target_link_libraries( saab_instrument_buf_iface_test saab_instrument_buf_iface )
