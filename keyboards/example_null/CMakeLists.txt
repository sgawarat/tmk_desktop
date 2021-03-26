add_library(keyboard STATIC
    keymap.cpp
)
target_link_libraries(keyboard PRIVATE
    config
)
