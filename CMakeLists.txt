cmake_minimum_required(VERSION 2.8)
project(AVOSCloud)

# set cxx flag
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # We want to link in C++11 mode if we're using Clang and on OS X.
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ftemplate-depth=256 -std=c++11 -stdlib=libc++")
  else()
    # We just add the -Wall and a high enough template depth
    # flag for Clang in other systems.
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ftemplate-dempth=256")
  endif()
endif()

if(BUILD_FOR_ARM)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ljsoncpp")
endif()

# set test option
if(WIN32 OR APPLE)
  option(BUILD_CppTests "Only build TestCpp sample" ON)
else()
  option(BUILD_CppTests "Only build TestCpp sample" ON)
endif()

# find OpenSSL, Threads, Boost, cppnetlib, jsoncpp
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTI_THREAD ON)

find_package( Boost 1.54.0
  REQUIRED unit_test_framework system regex date_time thread filesystem
  program_options chrono atomic log)

find_package( OpenSSL )
find_package( Threads )
find_package( cppnetlib 0.11.0 REQUIRED )

set(CPPNETLIB_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
set(CPPNETLIB_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcppnetlib-client-connections.a;${CMAKE_INSTALL_PREFIX}/lib/libcppnetlib-server-parsers.a;${CMAKE_INSTALL_PREFIX}/lib/libcppnetlib-uri.a)

set(JSONCPP_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libjsoncpp.a)

if (OPENSSL_FOUND)
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif (OPENSSL_FOUND)

if (Boost_FOUND)
  # include some dirs
  if(BUILD_FOR_ARM)
    include_directories(
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      )
  else()
    include_directories(
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${Boost_INCLUDE_DIRS}
      ${CPPNETLIB_INLUDE_DIRS}
      )
  endif()

  # AVOSCloud library
  add_subdirectory(src)

  # AVOSCloud headers
  install(DIRECTORY include DESTINATION ${CMAKE_BINARY_DIR})

  # cpp test
  if(BUILD_CppTests)
    enable_testing()
    add_subdirectory(tests)
  endif(BUILD_CppTests)
endif()
