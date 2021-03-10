add_subdirectory(settings)

add_library(hook SHARED
    hook.cpp
)
# target_compile_options(hook PRIVATE
#     -Wpedantic
# )
target_compile_definitions(hook PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
    TMK_DESKTOP_WIN32_HOOK_EXPORTS
)
target_link_libraries(hook PRIVATE
    config
)

add_executable(tmk_desktop WIN32
    main.cpp
    source.cpp
    sink.cpp
    host_driver.cpp
    key.cpp
    scancode.cpp
    main.rc
)
# target_compile_options(tmk_desktop PRIVATE
#     -Wpedantic
# )
target_compile_definitions(tmk_desktop PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
)
target_link_libraries(tmk_desktop PRIVATE
    imm32.lib
    config
    tmk_core
    engine
    hook
    settings
)
