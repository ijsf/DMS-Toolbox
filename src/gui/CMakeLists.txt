# -----------------------------------------------------------------------------
# - GUI library and executables                                               -
# -----------------------------------------------------------------------------
set(SOURCES
	${PROJECT_BINARY_DIR}/src/gui/generated.cpp
	mainframe.cc
	instpanel.cc
	envelopepanel.cc
	wavepanel.cc
	dmstb.cc
)

set(HEADERS
	${PROJECT_BINARY_DIR}/src/gui/generated.h
	mainframe.hh
	instpanel.hh
	envelopepanel.hh
	wavepanel.hh
)

add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/src/gui/generated.cpp
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/src/gui
	DEPENDS DMS-Toolbox.fbp
	COMMAND cp -f ${PROJECT_SOURCE_DIR}/src/gui/DMS-Toolbox.fbp .
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
target_compile_options(dmstb PRIVATE -Wno-effc++)

target_link_libraries(dmstb ${wxWidgets_LIBRARIES})
install(TARGETS dmstb
    RUNTIME DESTINATION bin
)
