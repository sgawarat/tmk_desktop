add_library(engine STATIC
    keyboard.cpp
    mapping.cpp
    host_driver.cpp
    misc.cpp
)
target_link_libraries(engine PRIVATE
    config
    tmk_core
)
