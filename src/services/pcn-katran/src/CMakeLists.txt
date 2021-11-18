set(CMAKE_CXX_STANDARD 17)
include(${PROJECT_SOURCE_DIR}/cmake/LoadFileAsVariable.cmake)

aux_source_directory(serializer SERIALIZER_SOURCES)
aux_source_directory(api API_SOURCES)
aux_source_directory(base BASE_SOURCES)

include_directories(serializer)

if (NOT DEFINED POLYCUBE_STANDALONE_SERVICE OR POLYCUBE_STANDALONE_SERVICE)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(POLYCUBE libpolycube)
  include_directories(${POLYCUBE_INCLUDE_DIRS})
endif(NOT DEFINED POLYCUBE_STANDALONE_SERVICE OR POLYCUBE_STANDALONE_SERVICE)

find_package(Boost REQUIRED)
find_package(folly REQUIRED)
find_package(Threads REQUIRED)
find_package(gflags REQUIRED)

include_directories(${folly_INCLUDE_DIRS})
set(FOLLY_LIBRARIES Folly::folly)

# Needed to load files as variables
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(pcn-katran SHARED
  ${SERIALIZER_SOURCES}
  ${API_SOURCES}
  ${BASE_SOURCES}
  Katran.cpp
  Ports.cpp
  Vip.cpp
  VipReal.cpp
  GeneralStats.cpp
  CHHelpers.cpp
  MaglevBase.cpp
  MaglevHash.cpp
  MaglevHashV2.cpp
  MurmurHash3.cpp
  Katran-lib.cpp
  IpHelpers.cpp
  MacHelpers.cpp)

# load ebpf datapath code a variable
load_file_as_variable(pcn-katran
  Katran_dp.c
  katran_code)

# load datamodel in a variable
load_file_as_variable(pcn-katran
  ../datamodel/katran.yang
  katran_datamodel)

target_link_libraries(pcn-katran ${POLYCUBE_LIBRARIES} ${FOLLY_LIBRARIES})

# Specify shared library install directory

set(CMAKE_INSTALL_LIBDIR /usr/lib)
set(KATRAN_EBPF_INCLUDE_PATH ${CMAKE_INSTALL_LIBDIR}/polycube/pcn-katran/ebpf_include)
set(KATRAN_EBPF_HEADER_INSTALL_PATH ${CMAKE_INSTALL_LIBDIR}/polycube/pcn-katran)

configure_file (Katran_config.h.in Katran_config.h @ONLY)

install(
    DIRECTORY ebpf_include
    DESTINATION "${KATRAN_EBPF_HEADER_INSTALL_PATH}"
)

install(
    TARGETS pcn-katran
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
