add_library(settings STATIC # SHARED
    # settings_null.cpp
    settings_example.cpp
)
# target_compile_options(settings PRIVATE
#     -Wpedantic
# )
target_compile_definitions(settings PRIVATE
    # TMK_SETTINGS_EXPORTS
)
target_link_libraries(settings PRIVATE
    config
    tmk_core
)
