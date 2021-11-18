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

# Needed to load files as variables
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(pcn-bmc SHARED
  ${SERIALIZER_SOURCES}
  ${API_SOURCES}
  ${BASE_SOURCES}
  Bmc.cpp
  BmcStats.cpp
  Memcached.cpp
  Ports.cpp
  Bmc-lib.cpp)

# load ebpf datapath code a variable
load_file_as_variable(pcn-bmc datapaths/BMC_hash_keys_dp.c bmc_hash_keys_dp)
load_file_as_variable(pcn-bmc datapaths/BMC_invalidate_cache_dp.c bmc_invalidate_cache_dp)
load_file_as_variable(pcn-bmc datapaths/BMC_prepare_packet_dp.c bmc_prepare_packet_dp)
load_file_as_variable(pcn-bmc datapaths/BMC_rx_filter_dp.c bmc_rx_filter_dp)
load_file_as_variable(pcn-bmc datapaths/BMC_tx_filter_dp.c bmc_tx_filter_dp)
load_file_as_variable(pcn-bmc datapaths/BMC_update_cache_dp.c bmc_update_cache_dp)
load_file_as_variable(pcn-bmc datapaths/BMC_write_reply_dp.c bmc_write_reply_dp)

# load datamodel in a variable
load_file_as_variable(pcn-bmc
  ../datamodel/bmc.yang
  bmc_datamodel)

target_link_libraries(pcn-bmc ${POLYCUBE_LIBRARIES})

# Specify shared library install directory

set(CMAKE_INSTALL_LIBDIR /usr/lib)
set(BMC_EBPF_INCLUDE_PATH ${CMAKE_INSTALL_LIBDIR}/polycube/pcn-bmc/ebpf_include)
set(BMC_EBPF_HEADER_INSTALL_PATH ${CMAKE_INSTALL_LIBDIR}/polycube/pcn-bmc)

configure_file (Bmc_config.h.in Bmc_config.h @ONLY)

install(
    DIRECTORY ebpf_include
    DESTINATION "${BMC_EBPF_HEADER_INSTALL_PATH}"
)

install(
    TARGETS pcn-bmc
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
