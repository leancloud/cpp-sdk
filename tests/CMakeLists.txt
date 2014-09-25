include_directories(
  ${AVOSCloud_SOURCE_DIR}/src
  ${Boost_INCLUDE_DIRS})

if (Boost_FOUND)
  set(
    TESTS
    AVPaasClientTest
    AVObjectTest
    AVQueryTest
    AVGeoPointTest
    AVFileTest
    AVUserTest
    AVACLTest
    )

  foreach(test ${TESTS})
    if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
      set_source_files_properties(${test}.cpp
        PROPERTIES COMPILE_FLAGS "-Wall")
    endif()

    add_executable(AVOSCloud-${test} ${test}.cpp)
    add_dependencies(AVOSCloud-${test} avoscloud)
    target_link_libraries(AVOSCloud-${test} ${Boost_LIBRARIES} avoscloud)

    if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      target_link_libraries(AVOSCloud-${test} ws2_32 wsock32)
    endif()
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      target_link_libraries(AVOSCloud-${test} rt)
    endif()
    set_target_properties(AVOSCloud-${test}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${AVOSCloud_BINARY_DIR}/bin)
    add_test(AVOSCloud-${test}
      ${AVOSCloud_BINARY_DIR}/bin/AVOSCloud-${test})
  endforeach()

endif()
