set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory (googletest EXCLUDE_FROM_ALL)

set(target_SRC
    test_desturction_order.cpp
    test_mocking_example.cpp
)

add_executable(cpp_examples_tests
    ${target_SRC}
)

target_link_libraries(cpp_examples_tests
    gmock
    gtest_main
)
