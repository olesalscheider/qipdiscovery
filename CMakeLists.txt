project(qipdiscovery)

cmake_minimum_required(VERSION 3.0)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(SO_VERSION 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(Qt5 REQUIRED COMPONENTS Core Network)
find_package(Miniupnpc REQUIRED)

include(GNUInstallDirs)

include_directories(
    inc
    ${MINIUPNPC_INCLUDE_DIRS}
)

set(qipdiscovery_SOURCES
    src/qipdiscoverymanager.cc
    src/qipdiscoverydebug.cc
    src/qipendpoint.cc
    src/qipendpointsimple.cc
    src/qipendpointigdforwarding.cc
)

set(qipdiscovery_INSTALL_HEADERS
    inc/qipdiscoverymanager.hh
    inc/qipendpoint.hh
)

add_library(qipdiscovery SHARED ${qipdiscovery_SOURCES})
set_target_properties(qipdiscovery PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${SO_VERSION}
    EXPORT_NAME QIPDiscovery
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_INSTALL_FULL_INCLUDEDIR}/qipdiscovery"
)

target_link_libraries(qipdiscovery
    PUBLIC
    Qt5::Core
    Qt5::Network
    PRIVATE
    ${MINIUPNPC_LIBRARIES}
)

install(
    TARGETS qipdiscovery
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    EXPORT QIPDiscoveryTarget
)

install(
    EXPORT QIPDiscoveryTarget
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/qipdiscovery"
    FILE QIPDiscovery.cmake
    NAMESPACE QIPDiscovery::
    COMPONENT Devel
)

install(
    FILES QIPDiscoveryConfig.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/qipdiscovery"
    COMPONENT Devel
)

install(
    FILES ${qipdiscovery_INSTALL_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qipdiscovery"
)

