include_directories(${CMAKE_SOURCE_DIR}/src/libs/polycube/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/spdlog)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/bcc/src/cc/)

set(POLYCUBE_STANDALONE_SERVICE false)
set(POLYCUBE_LIBRARIES polycube uuid tins)

set(LOAD_SERVICES "" PARENT_SCOPE)

macro(add_service servicename servicefolder)
    string(TOUPPER ${servicename} servicenameupper)
    option(ENABLE_SERVICE_${servicenameupper} "compiles the ${servicename} service" ON)
    if (ENABLE_SERVICE_${servicenameupper})
        add_subdirectory(${servicefolder})
        string(CONCAT LOAD_SERVICES "${LOAD_SERVICES}" "try_to_load(\"${servicename}\", \"libpcn-${servicename}.so\");\n")
    endif (ENABLE_SERVICE_${servicenameupper})
endmacro()

# put your new service here
# first argument is the service name used in the rest API
# second argument is the folder of the service

add_service(bridge pcn-bridge)
add_service(nat pcn-nat)
add_service(router pcn-router)
add_service(iptables pcn-iptables)
add_service(katran pcn-katran)
#add_service(bmc pcn-bmc)

# save string to create code that load the services
SET_PROPERTY(GLOBAL PROPERTY LOAD_SERVICES_ ${LOAD_SERVICES})

install(DIRECTORY datamodel-common DESTINATION include/polycube)
