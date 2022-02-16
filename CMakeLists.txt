cmake_minimum_required(VERSION 3.12)

project(RTT VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include_directories(include)
include_directories(libs)

find_package(nlohmann_json REQUIRED)
find_package(MySQL REQUIRED)

add_library(cephes STATIC
  libs/cephes/cephes.cpp
)
target_include_directories(cephes PUBLIC libs/cephes)
add_library(cephes::cephes ALIAS cephes)

add_executable(randomness_testing_toolkit
  src/batteries/batteryoutput.cpp
  src/batteries/configuration-batt.cpp
  src/batteries/dieharder/battery-dh.cpp
  src/batteries/dieharder/setting-dh.cpp
  src/batteries/dieharder/test-dh.cpp
  src/batteries/dieharder/testresult-dh.cpp
  src/batteries/dieharder/variant-dh.cpp
  src/batteries/ibattery-batt.cpp
  src/batteries/itest-batt.cpp
  src/batteries/itestresult-batt.cpp
  src/batteries/ivariant-batt.cpp
  src/batteries/niststs/battery-sts.cpp
  src/batteries/niststs/test-sts.cpp
  src/batteries/niststs/testresult-sts.cpp
  src/batteries/niststs/variant-sts.cpp
  src/batteries/result/statistic-res.cpp
  src/batteries/result/subtestresult-res.cpp
  src/batteries/result/variantresult-res.cpp
  src/batteries/testconstants.cpp
  src/batteries/testrunner-batt.cpp
  src/batteries/testu01/battery-tu01.cpp
  src/batteries/testu01/test-tu01.cpp
  src/batteries/testu01/testresult-tu01.cpp
  src/batteries/testu01/variant-tu01.cpp
  src/clinterface/batteryarg.cpp
  src/clinterface/resultstoragearg.cpp
  src/clinterface/rttclioptions.cpp
  src/constants.cpp
  src/globalcontainer.cpp
  src/logger.cpp
  src/main.cpp
  src/storage/filestorage.cpp
  src/storage/istorage.cpp
  $<$<BOOL:${rtt_USE_SQL}>:src/storage/mysqlstorage.cpp>
  src/strings.cpp
  src/toolkitsettings.cpp
  src/utils.cpp
)
target_include_directories(randomness_testing_toolkit PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}libs/moderncppjson>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}include>
  PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(randomness_testing_toolkit PUBLIC
  cephes::cephes
  nlohmann_json::nlohmann_json
  $<$<BOOL:${rtt_USE_SQL}>:mysqlcppconn>
)

# install binary
install(TARGETS randomness_testing_toolkit
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)

# install regular files
install(FILES LICENSE DESTINATION .)
install(DIRECTORY configuration_files DESTINATION .
  FILES_MATCHING PATTERN "*.json"
)
