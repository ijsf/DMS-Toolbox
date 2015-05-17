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
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
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
