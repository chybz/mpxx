include_directories(
    /Users/chybz/dev/mpxx/sources/tests/mpxx_msg
    /Users/chybz/dev/mpxx/build/include.generated
    /Users/chybz/dev/mpxx/sources/include/mpxx
)

add_executable(
    tst_mpxx_msg
    test.cpp
)

set_target_properties(
    tst_mpxx_msg
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY /Users/chybz/dev/mpxx/build/t
    RUNTIME_OUTPUT_DIRECTORY_DEBUG /Users/chybz/dev/mpxx/build/t
    RUNTIME_OUTPUT_DIRECTORY_RELEASE /Users/chybz/dev/mpxx/build/t
    OUTPUT_NAME mpxx_msg.t
)

target_link_libraries(
    tst_mpxx_msg
    msgpack
)
