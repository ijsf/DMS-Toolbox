# -----------------------------------------------------------------------------
# - GUI library and executables                                               -
# -----------------------------------------------------------------------------
set(SOURCES
	generated.cpp
	mainframe.cc
	instpanel.cc
	envelopepanel.cc
	wavepanel.cc
	dmstb.cc
)

set(HEADERS
	generated.h
	gui.hh
	mainframe.hh
	instpanel.hh
	envelopepanel.hh
	wavepanel.hh
)

file(COPY
	DMS-Toolbox.fbp
	DESTINATION .
)

add_custom_command(
	OUTPUT generated.cpp
	DEPENDS DMS-Toolbox.fbp
	COMMAND wxformbuilder -g DMS-Toolbox.fbp
)

include_directories("${PROJECT_BINARY_DIR}/src")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_executable(dmstb WIN32 ${SOURCES}
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:wersi>
)
else()
add_executable(dmstb ${SOURCES}
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:wersi>
)
endif()
# TODO need this due to generated code not being so ultra-clean
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(dmstb PRIVATE -Wno-effc++)
endif()

target_link_libraries(dmstb ${wxWidgets_LIBRARIES})
install(TARGETS dmstb
    RUNTIME DESTINATION bin
)
