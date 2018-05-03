cmake_minimum_required(VERSION 3.8)
project(VCS)

set(CMAKE_CXX_STANDARD 14)


# Build source files as library that can easily be linked to both the executable and the tests.
set(SOURCE_FILES VersionControl/Commit.cpp VersionControl/Commit.h Routing/Router.cpp Routing/Router.h Routing/Command.cpp Routing/Command.h Actions/HelpAction.cpp Actions/HelpAction.h Actions/Action.cpp Actions/Action.h Management/Storage.cpp Management/Storage.h Actions/InitAction.cpp Actions/InitAction.h Config.cpp Config.h Actions/CommitAction.cpp Actions/CommitAction.h Utils.cpp Utils.h VersionControl/Diff.cpp VersionControl/Diff.h Actions/AuthorAction.cpp Actions/AuthorAction.h Actions/ShowAction.cpp Actions/ShowAction.h Actions/GraphAction.cpp Actions/GraphAction.h Actions/CheckoutAction.cpp Actions/CheckoutAction.h VersionControl/Helper.cpp VersionControl/Helper.h Actions/MergeAction.cpp Actions/MergeAction.h)
add_library(vcs_lib ${SOURCE_FILES})

include_directories(.)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS REQUIRED system filesystem serialization)

target_link_libraries(vcs_lib ${Boost_LIBRARIES})


# Build main executable that is linked to the above library.
add_executable(vcs main.cpp)
target_link_libraries(vcs vcs_lib)


# Also build tests.
add_subdirectory(Tests)
