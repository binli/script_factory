project(DianPing-scope CXX)
cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")
set(UBUNTU_PROJECT_TYPE "Scope" CACHE INTERNAL "Tells QtCreator this is a Scope project")

include(GNUInstallDirs)
set(SCOPE_NAME DianPing-scope)
set(INI_INSTALLDIR ${CMAKE_INSTALL_LIBDIR}/unity-scopes/${SCOPE_NAME})
set(SCOPE_INSTALLDIR ${CMAKE_INSTALL_LIBDIR}/unity-scopes/${SCOPE_NAME})

include(FindPkgConfig)
pkg_check_modules(UNITY_SCOPES libunity-scopes>=0.4.0 REQUIRED)

add_subdirectory(src)
add_subdirectory(data)
enable_testing()
add_subdirectory(test)
