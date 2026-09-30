add_library(pe_core STATIC
    src/log.cpp
    src/time.cpp
)

target_include_directories(pe_core
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(pe_core
    PUBLIC  SDL3::SDL3
    PRIVATE pe_warnings
)

set_target_properties(pe_core PROPERTIES FOLDER "engine")