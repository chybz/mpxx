include_directories(
    ../../../build/include.generated
    ../..//Users/chybz/dev/mpxx/sources/include/mpxx
)

add_executable(
    tst_mpxx_msg
    test.cpp
)

set_target_properties(
    tst_mpxx_msg
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../../t
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ../../t
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ../../t
    OUTPUT_NAME mpxx_msg.t
)

target_link_libraries(
    tst_mpxx_msg
    msgpack
)
