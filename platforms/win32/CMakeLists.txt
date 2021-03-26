add_executable(tmk_desktop WIN32
    main.cpp
    main.rc
)
target_link_libraries(tmk_desktop PRIVATE
    config
    engine
    keyboard
)
