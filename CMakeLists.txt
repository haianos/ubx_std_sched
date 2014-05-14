CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ubx_std_sched)

set(CMAKE_CXX_FLAGS "-Wall -Werror -fvisibility=hidden")
set(CMAKE_CXX_COMPILER clang++ )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake") ## temporary resources, for UBX

# Global
set(INSTALL_LIB_TYPES_DIR lib/ubx/types CACHE PATH "Installation directory for types libraries")
set(INSTALL_LIB_BLOCKS_DIR lib/ubx/blocks CACHE PATH "Installation directory for blocks libraries")
set(INSTALL_BIN_APPS_DIR bin CACHE PATH "Installation directory for bin")
set(INSTALL_INCLUDE_DIR include/ubx CACHE PATH "Installation directory for header files (types)")
set(INSTALL_CMAKE_DIR  share/ubx/cmake CACHE PATH  "Installation directory for CMake files") # default ${DEF_INSTALL_CMAKE_DIR}

# Make relative paths absolute
foreach(p LIB_TYPES LIB_BLOCKS BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

##
# Add uninstall target.
##
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
    
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
)

##
# Generate config package
##
set(CONF_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")
set(CONF_CMAKE_DIR "${INSTALL_CMAKE_DIR}")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ubx-std-sched-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/ubx-std-sched-config.cmake" @ONLY
)

set (UBX_ROOT $ENV{UBX_ROOT} )
if (NOT UBX_ROOT)
 message( FATAL_ERROR "UBX_ROOT is not defined. CMake install willl abort." )
endif()

find_package(Ubx REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${UBX_INCLUDE_DIR}
)
message("supermega working follows")
message(${UBX_INCLUDE_DIR})
set(GEN_HEXARR ${UBX_ROOT}/tools/file2carr.lua)

file(GLOB types
  ${CMAKE_CURRENT_SOURCE_DIR}/types/*.h
)


set(outfiles "")
foreach( _type ${types} )
  string(REPLACE ".h" ".h.hexarr" _outfile ${_type})
  add_custom_command(
  OUTPUT ${_outfile}
  COMMAND ${GEN_HEXARR}
  ARGS ${_type}  > ${_outfile}
  )
  list(APPEND outfiles ${_outfile})
endforeach(_type)
add_custom_target( gen_hexarr ALL DEPENDS ${outfiles} )

add_library(ubx_std_sched_types SHARED types/ubx_std_sched_types.c)
set_target_properties(ubx_std_sched_types PROPERTIES PREFIX "") 
set_property(TARGET ubx_std_sched_types PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(ubx_std_sched_types ${UBX_LIBRARIES})
add_dependencies(ubx_std_sched_types gen_hexarr)

### Install libraries
set_target_properties(ubx_std_sched_types PROPERTIES PUBLIC_HEADER "${outfiles};${types}" )

install(TARGETS ubx_std_sched_types 
  EXPORT ubx-std-sched-types-targets
  LIBRARY DESTINATION "${INSTALL_LIB_TYPES_DIR}" COMPONENT ubx_std_sched_types
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/types" COMPONENT dev
)

# Install the ubx_std_sched_types-config.cmake 
install(FILES
  "${PROJECT_BINARY_DIR}/cmake/ubx-std-sched-config.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT ubx-std-sched-types-targets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
  
# Compile library schedmod
add_library(schedmod SHARED ..//ubx_std_sched/sseq/sseq.c
..//ubx_std_sched/modules/schedmod_module.c
)
set_target_properties(schedmod PROPERTIES PREFIX "")
target_link_libraries(schedmod ${UBX_LIBRARIES})
add_dependencies(schedmod gen_hexarr)

# Install schedmod
install(TARGETS schedmod DESTINATION ${INSTALL_LIB_BLOCKS_DIR} EXPORT schedmod-block)
set_property(TARGET schedmod PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
install(EXPORT schedmod-block DESTINATION ${INSTALL_CMAKE_DIR})

# Compile library schedmod
add_library(dummiesmod SHARED ..//ubx_std_sched/dummies/receiver.c
..//ubx_std_sched/modules/dummies_module.c
)
set_target_properties(dummiesmod PROPERTIES PREFIX "")
target_link_libraries(dummiesmod ${UBX_LIBRARIES})
add_dependencies(dummiesmod gen_hexarr)

# Install dummiesmod
install(TARGETS dummiesmod DESTINATION ${INSTALL_LIB_BLOCKS_DIR} EXPORT dummiesmod-block)
set_property(TARGET dummiesmod PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
install(EXPORT dummiesmod-block DESTINATION ${INSTALL_CMAKE_DIR})

add_executable(sseq src_bin/sseq)
target_link_libraries(sseq ${UBX_LIBRARIES})
add_dependencies(sseq gen_hexarr)