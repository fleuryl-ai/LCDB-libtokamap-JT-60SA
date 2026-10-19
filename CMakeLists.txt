cmake_minimum_required(VERSION 3.20)

project(
    uda_lcdb_plugin
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    LIBTOKAMAP_ROOT
    /analysis/easybuild/opt/libtokamap/0.2.5-foss-2025b
)

set(
    UDA_ROOT
    /analysis/easybuild/opt/EasyBuild/software/UDA/2.9.3-GCC-14.3.0
)

#
# UDA plugin
#
add_library(
    uda_lcdb_plugin
    SHARED
    src/plugin/uda_lcdb_plugin.cpp
    src/lcdb/lcdb_client.cpp
    src/datasource/lcdb_data_source.cpp
)

target_include_directories(
    uda_lcdb_plugin
    PRIVATE
        ${UDA_ROOT}/include/uda
        ${PROJECT_SOURCE_DIR}/src
        ${LIBTOKAMAP_ROOT}/include
	${LIBTOKAMAP_ROOT}/include/libtokamap
)

target_link_directories(
    uda_lcdb_plugin
    PRIVATE
        ${UDA_ROOT}/lib
)

target_link_libraries(
    uda_lcdb_plugin
    PRIVATE
        dl
        /analysis/lib/liblcdb.so
        ${LIBTOKAMAP_ROOT}/lib/libtokamap.so
	uda_plugins
)

set_target_properties(
    uda_lcdb_plugin
    PROPERTIES
        BUILD_RPATH
            "/analysis/lib;\
${LIBTOKAMAP_ROOT}/lib;\
${UDA_ROOT}/lib;\
/home/ap/MATLAB/R2025b/sys/os/glnxa64;\
/home/ap/intel/oneapi/compiler/2025.3/lib"

        INSTALL_RPATH
            "/analysis/lib;\
${LIBTOKAMAP_ROOT}/lib;\
${UDA_ROOT}/lib;\
/home/ap/MATLAB/R2025b/sys/os/glnxa64;\
/home/ap/intel/oneapi/compiler/2025.3/lib"
)

target_compile_definitions(
    uda_lcdb_plugin
    PRIVATE
    # LCDB_DEFAULT_CONFIG="/analysis/easybuild/shared/lcdb_test/config.toml"
    LCDB_DEFAULT_CONFIG="${CMAKE_SOURCE_DIR}/config/config.toml"
)

#
# Unit Tests
#
add_executable(
    lcdb_tests
    test/test_main.cpp
    test/test_lcdb.cpp
    test/test_lcdb_client.cpp
    test/test_lcdb_data_source.cpp
    test/test_mapping_handler.cpp
    test/test_uda_lcdb_plugin.cpp
    test/test_plugin_helper.cpp
    test/test_versioning.cpp
    src/lcdb/lcdb_client.cpp
    src/datasource/lcdb_data_source.cpp
)

target_include_directories(
    lcdb_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${LIBTOKAMAP_ROOT}/include
        ${LIBTOKAMAP_ROOT}/include/libtokamap
	${UDA_ROOT}/include/uda
)

target_link_libraries(
    lcdb_tests
    PRIVATE
        dl
    	/analysis/lib/liblcdb.so
    	${LIBTOKAMAP_ROOT}/lib/libtokamap.so
	uda_lcdb_plugin
	uda_plugins
)

target_compile_definitions(
    lcdb_tests
    PRIVATE
    LCDB_DEFAULT_CONFIG="${CMAKE_SOURCE_DIR}/config/config.toml"
)

