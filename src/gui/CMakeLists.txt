# -----------------------------------------------------------------------------
# - GUI library and executables                                               -
# -----------------------------------------------------------------------------
set(SOURCES
	generated.cpp
	mainframe.cc
	dmstb.cc
)

set(HEADERS
	generated.h
	mainframe.hh
)

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
