
add_executable(single-thread-test single-thread.cpp)
add_executable(multi-thread-test multi-thread.cpp)

target_link_libraries(multi-thread-test pthread)

add_library(multi-thread SHARED multi-thread-so.cpp)
target_link_libraries(multi-thread pthread)
add_executable(starter-test starter.cpp)
target_link_libraries(starter-test dl)