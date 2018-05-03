include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Build test executable.
add_executable(vcs_integration_tests InitTest.cpp CommitTest.cpp CheckoutTest.cpp MergeTest.cpp)

# Link wiht Google Test and the main source library.
target_link_libraries(vcs_integration_tests gtest gtest_main)
target_link_libraries(vcs_integration_tests vcs_lib)
