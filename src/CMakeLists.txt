# -----------------------------------------------------------------------------
# - Build toolchain configuration                                             -
# -----------------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Weffc++ -std=c++0x -pedantic")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Weffc++ -std=c++0x -pedantic")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreMIDI -framework CoreFoundation -framework CoreAudio -lrtmidi")
endif()
include_directories("${PROJECT_SOURCE_DIR}/src")

# -----------------------------------------------------------------------------
# - Check for wxWidgets                                                       -
# -----------------------------------------------------------------------------
set(wxWidgets_CONFIGURATION mswud)
find_package(wxWidgets COMPONENTS core base adv)
if(wxWidgets_FOUND)
    set(BUILD_GUI 1)
    include("${wxWidgets_USE_FILE}")
endif(wxWidgets_FOUND)

# -----------------------------------------------------------------------------
# - Check for RtMidi                                                          -
# -----------------------------------------------------------------------------
find_path(RTMIDI_INCLUDE_DIR
    NAMES RtMidi.h
    PATHS /usr/include /usr/local/include /opt/local/include "${RTMIDI_ROOT_DIR}"
)
set(RTMIDI_LIBNAME rtmidi)
if(WIN32)
    set(RTMIDI_LIBNAME rtmidi.lib)
endif(WIN32)
find_library(RTMIDI_LIBRARY
    NAMES ${RTMIDI_LIBNAME}
    PATHS /usr/lib /usr/local/lib /opt/local/lib "${RTMIDI_ROOT_DIR}/i386"
)
if(WIN32 AND RTMIDI_LIBRARY)
  get_filename_component(rtmidi_lib_path ${RTMIDI_LIBRARY} PATH)
  set(RTMIDI_DLL ${rtmidi_lib_path}/rtmidi.dll)
endif(WIN32 AND RTMIDI_LIBRARY)
if(RTMIDI_LIBRARY)
    if(RTMIDI_INCLUDE_DIR)
        set(RTMIDI_FOUND TRUE)
        message(STATUS "Found RtMidi")
    else(RTMIDI_INCLUDE_DIR)
        set(RTMIDI_FOUND FALSE)
    endif(RTMIDI_INCLUDE_DIR)
else(RTMIDI_LIBRARY)
    set(RTMIDI_FOUND FALSE)
endif(RTMIDI_LIBRARY)
if(RTMIDI_FOUND)
    include_directories("${RTMIDI_INCLUDE_DIR}")
    add_definitions(-DHAVE_RTMIDI)
endif(RTMIDI_FOUND)

# -----------------------------------------------------------------------------
# - Core library                                                              -
# -----------------------------------------------------------------------------
set(SOURCES
	exceptions.cc
)

set(HEADERS
	common.hh
	exceptions.hh
)

add_library(core OBJECT ${SOURCES})

# -----------------------------------------------------------------------------
# - Other libraries                                                           -
# -----------------------------------------------------------------------------
add_subdirectory(wersi)

# -----------------------------------------------------------------------------
# - Non-GUI executables                                                       -
# -----------------------------------------------------------------------------
add_executable(dmsdump dmsdump.cc
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:wersi>
)
install(TARGETS dmsdump
    RUNTIME DESTINATION bin
)

# -----------------------------------------------------------------------------
# - GUI libraries/executables                                                 -
# -----------------------------------------------------------------------------
if(BUILD_GUI)
    add_subdirectory(gui)
endif(BUILD_GUI)
