add_library(engine_impl STATIC
    scancode.cpp
)
target_link_libraries(engine_impl PRIVATE
    config
)
