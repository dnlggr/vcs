include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Build test executable.
add_executable(vcs_unit_tests StorageTest.cpp ShellTest.cpp DiffTests.cpp)

# Link wiht Google Test and the main source library.
target_link_libraries(vcs_unit_tests gtest gtest_main)
target_link_libraries(vcs_unit_tests vcs_lib)
