add_library(tmk_core STATIC
    tmk_core/common/host.c
    tmk_core/common/keyboard.c
    tmk_core/common/matrix.c
    tmk_core/common/action.c
    tmk_core/common/action_tapping.c
    tmk_core/common/action_macro.c
    tmk_core/common/action_layer.c
    tmk_core/common/action_util.c
    # tmk_core/common/print.c
    tmk_core/common/debug.c
    tmk_core/common/util.c
    tmk_core/common/hook.c
)
target_include_directories(tmk_core INTERFACE
    .
)
target_link_libraries(tmk_core PRIVATE
    config
)
