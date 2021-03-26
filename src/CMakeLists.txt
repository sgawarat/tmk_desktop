if(WIN32)
    add_subdirectory(win32)
endif()

add_library(engine STATIC
    ${TMK_CORE_DIR}/common/host.c
    ${TMK_CORE_DIR}/common/keyboard.c
    ${TMK_CORE_DIR}/common/matrix.c
    ${TMK_CORE_DIR}/common/action.c
    ${TMK_CORE_DIR}/common/action_tapping.c
    ${TMK_CORE_DIR}/common/action_macro.c
    ${TMK_CORE_DIR}/common/action_layer.c
    ${TMK_CORE_DIR}/common/action_util.c
    ${TMK_CORE_DIR}/common/print.c
    ${TMK_CORE_DIR}/common/debug.c
    ${TMK_CORE_DIR}/common/util.c
    ${TMK_CORE_DIR}/common/hook.c
    ${TMK_CORE_DIR}/common/actionmap.c

    source.cpp
    keyboard.cpp
    sink.cpp
    timer.cpp
    wait.cpp
)
target_link_libraries(engine PRIVATE
    config
    engine_impl
)
