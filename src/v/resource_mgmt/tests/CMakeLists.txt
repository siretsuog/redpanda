rp_test(
  UNIT_TEST
  BINARY_NAME resource_mgmt
  SOURCES
    available_memory_test.cc
  LIBRARIES v::seastar_testing_main v::resource_mgmt v::config
  LABELS resource_mgmt
)

# NB: Some of these rely on global state (low watermark of available_memory) so need to run in a separate binary
rp_test(
        UNIT_TEST
        BINARY_NAME test_memory_sampling
        SOURCES memory_sampling_tests.cc
        DEFINITIONS BOOST_TEST_DYN_LINK
        LIBRARIES v::seastar_testing_main v::application
        LABELS memory_sampling
        SKIP_BUILD_TYPES "Debug"
)

rp_test(
        UNIT_TEST
        BINARY_NAME test_memory_sampling_reclaimer
        SOURCES memory_sampling_reclaimer_test.cc
        DEFINITIONS BOOST_TEST_DYN_LINK
        LIBRARIES v::seastar_testing_main v::application
        LABELS memory_sampling
        SKIP_BUILD_TYPES "Debug"
)
