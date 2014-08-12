# Put the ini file in the build directory next to the scope
# .so file so test tools can find both easily.
configure_file(DianPing-scope.ini.in ../src/DianPing-scope.ini)

INSTALL(FILES ${CMAKE_BINARY_DIR}/src/DianPing-scope.ini
DESTINATION ${INI_INSTALLDIR})
