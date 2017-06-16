CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT(pathtracer)

# Configure
IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release ... FORCE)
ENDIF()

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  MESSAGE("INFO: Debug build")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
  MESSAGE("INFO: Release build")
ELSE()
  MESSAGE("WARNING: Unknown build type")
ENDIF()

# Includes
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(lib/include)
INCLUDE_DIRECTORIES(lib/src/spdlog/include)

# Sub-projects/libraries
ADD_SUBDIRECTORY(lib/src/spdlog)

# Files
FILE(GLOB CORE    "src/*.cpp")
FILE(GLOB HEADERS "include/*.hpp")
FILE(GLOB CLIENT  "src/main.cpp")
FILE(GLOB TESTS   "tests/*.cpp")
LIST(REMOVE_ITEM CORE "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Targets
ADD_LIBRARY(core ${CORE} ${HEADERS})
ADD_EXECUTABLE(pathtracer ${CLIENT} ${HEADERS})
ADD_EXECUTABLE(pathtracer_tests ${TESTS} ${HEADERS})

# Tests
ENABLE_TESTING()
ADD_TEST(NAME all COMMAND pathtracer_tests)

# Link core lib
TARGET_LINK_LIBRARIES(pathtracer core)
TARGET_LINK_LIBRARIES(pathtracer_tests core)

# Link thread lib
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(pathtracer ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(pathtracer_tests ${CMAKE_THREAD_LIBS_INIT})
