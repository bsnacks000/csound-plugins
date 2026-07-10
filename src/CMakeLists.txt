# Csound opcode build

if(APPLE)
  find_library(CSOUND_LIBRARY CsoundLib
        PATHS
            ~/Library/Frameworks/CsoundLib.framework/CsoundLib
    )

  # Frameworks keep headers in:
  #   CsoundLib.framework/Headers
  # so we find them there
  find_path(CSOUND_INCLUDE_DIR csound.h
        PATHS
            ~/Library/Frameworks/CsoundLib.framework/Headers
    )
else()
  message(STATUS "Building Csound on linux...")
  find_library(CSOUND_LIBRARY csound
        PATHS /usr/local/lib
    )

  find_path(CSOUND_INCLUDE_DIR csound.h
        PATHS /usr/local/include/csound/
    )
endif()

if(NOT CSOUND_INCLUDE_DIR OR NOT CSOUND_LIBRARY)
  message(FATAL_ERROR "Csound not found!")
endif()

message(STATUS "csound: include dir: ${CSOUND_INCLUDE_DIR}")
message(STATUS "csound: library: ${CSOUND_LIBRARY}")

set(CSND_PLUGIN_LIB blpcsnd)
file(GLOB PLUGIN_SRC_FILES *.c)
add_library(${CSND_PLUGIN_LIB} SHARED ${PLUGIN_SRC_FILES})


target_compile_options(${CSND_PLUGIN_LIB} PRIVATE -Wextra -Wall)
target_link_options(${CSND_PLUGIN_LIB} PRIVATE -Wno-undef)

target_include_directories(${CSND_PLUGIN_LIB} PRIVATE
    ${CSOUND_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(${CSND_PLUGIN_LIB} ${CSOUND_LIBRARY} dsp::objects)

# lto
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_err)

if (lto_supported)
  message(STATUS "csound: LTO enabled")
  set_property(TARGET ${CSND_PLUGIN_LIB} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
