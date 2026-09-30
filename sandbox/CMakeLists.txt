add_executable(sandbox src/main.cpp)

target_link_libraries(sandbox
    PRIVATE pe_core
            pe_warnings
)

set_target_properties(sandbox PROPERTIES FOLDER "apps")

# SDL3 is a DLL on Windows; it must sit next to the .exe at runtime.
# $<TARGET_RUNTIME_DLLS:...> is a generator expression — CMake resolves it at
# build time, when it knows exactly which DLLs the target needs.
if(WIN32)
    add_custom_command(TARGET sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:sandbox> $<TARGET_FILE_DIR:sandbox>
        COMMAND_EXPAND_LISTS
    )
endif()
