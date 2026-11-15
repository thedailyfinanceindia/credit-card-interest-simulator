cmake_minimum_required(VERSION 3.20)

project(credit_card_interest_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(simulator
    src/calculation/interest_calculator.cpp
    src/calculation/main_scenario_calculator.cpp
    src/calculation/missed_payment_simulator.cpp
    src/calculation/percentage_calculator.cpp
    src/calculation/tax_calculator.cpp
    src/domain/money.cpp
    src/domain/rate.cpp
    src/validation/input_validator.cpp
)

target_include_directories(simulator PUBLIC src)
target_compile_features(simulator PUBLIC cxx_std_20)

add_library(simulator_api OBJECT src/app/wasm_api.cpp)
target_include_directories(simulator_api PRIVATE src)
target_compile_features(simulator_api PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(simulator PRIVATE /W4 /WX /permissive-)
    target_compile_options(simulator_api PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(simulator PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(simulator_api PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(simulator_tests tests/unit/simulator_tests.cpp)
    target_link_libraries(simulator_tests PRIVATE simulator simulator_api)
    if(MSVC)
        target_compile_options(simulator_tests PRIVATE /W4 /WX /permissive-)
    else()
        target_compile_options(simulator_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME simulator_tests COMMAND simulator_tests)
endif()

if(EMSCRIPTEN)
    add_executable(simulator_web $<TARGET_OBJECTS:simulator_api>)
    target_link_libraries(simulator_web PRIVATE simulator)
    set_target_properties(simulator_web PROPERTIES
        OUTPUT_NAME simulator
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/site"
        SUFFIX ".js"
    )
    target_link_options(simulator_web PRIVATE
        "--no-entry"
        "-sMODULARIZE=1"
        "-sEXPORT_NAME=createSimulatorModule"
        "-sENVIRONMENT=web"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXPORTED_FUNCTIONS=['_ccsim_main_result','_ccsim_missed_result','_malloc','_free']"
        "-sEXPORTED_RUNTIME_METHODS=['UTF8ToString','lengthBytesUTF8','stringToUTF8']"
    )
    add_custom_command(TARGET simulator_web POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:simulator_web>"
            "${CMAKE_SOURCE_DIR}/web/simulator.js"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE_DIR:simulator_web>/simulator.wasm"
            "${CMAKE_SOURCE_DIR}/web/simulator.wasm"
        VERBATIM
    )
    add_custom_target(simulator_web_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/web"
            "${CMAKE_BINARY_DIR}/site"
    )
    add_dependencies(simulator_web simulator_web_assets)
endif()
