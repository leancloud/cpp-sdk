set(AVOSCloud_SRC
  AVOSCloud.cpp
  )

include(ACL/CMakeLists.txt)
include(AVFile/CMakeLists.txt)
include(AVObject/CMakeLists.txt)
include(Geo/CMakeLists.txt)
include(Query/CMakeLists.txt)
include(Request/CMakeLists.txt)
include(User/CMakeLists.txt)
include(Utils/CMakeLists.txt)

add_library(avoscloud STATIC
  ${AVOSCloud_SRC}
  ${AVOSCloud_ACL_SRC}
  ${AVOSCloud_AVObject_SRC}
  ${AVOSCloud_AVFile_SRC}
  ${AVOSCloud_Geo_SRC}
  ${AVOSCloud_Query_SRC}
  ${AVOSCloud_Request_SRC}
  ${AVOSCloud_User_SRC}
  ${AVOSCloud_Utils_SRC}
  )

set(BOOST_CLIENT_LIBS
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_CHRONO_LIBRARY}
  ${Boost_ATOMIC_LIBRARY}
  ${Boost_LOG_LIBRARY}
  )

target_link_libraries( avoscloud
  ${CPPNETLIB_LIBRARIES}
  ${JSONCPP_LIBRARIES}
  ${BOOST_CLIENT_LIBS}
  )

if (OPENSSL_FOUND)
  target_link_libraries(avoscloud ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

set_target_properties( avoscloud
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
