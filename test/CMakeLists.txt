set(SOURCES
  "hello_test.cpp"
  "../src/SignalProcessor.cpp"
  "../src/ProblemConfig.cpp"
  "../src/SpikesObserver.cpp"
)
set (INCLUDE_DIR ../include )

find_package(Sanitizers)

set(TEST_NAME tests)

add_executable(${TEST_NAME} test_main.cpp ${SOURCES})
add_sanitizers(${TEST_NAME})
target_include_directories(${TEST_NAME} PUBLIC ${INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR})
target_link_libraries(${TEST_NAME} PRIVATE ${YAMLCPP_LIBRARY} catch)
target_compile_definitions(${TEST_NAME} PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=60)

if (MSVC)
  target_compile_options(${TEST_NAME} PRIVATE "/IW4")
  target_compile_options(${TEST_NAME} PRIVATE "/WX")
else()
  target_compile_options(${TEST_NAME} PRIVATE "-Wall")
  target_compile_options(${TEST_NAME} PRIVATE "-Wextra")
  target_compile_options(${TEST_NAME} PRIVATE "-Werror")
  target_compile_options(${TEST_NAME} PRIVATE "-pedantic")
endif()
