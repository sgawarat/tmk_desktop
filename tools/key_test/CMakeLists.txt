add_executable(key_test WIN32
    main.cpp
)
target_link_libraries(key_test PRIVATE
    config
)
