cmake_minimum_required(VERSION 3.8)

project(connect_four)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

####################
# Source files
####################

set(HEADER_FILES
	src/ComputerPlayer.hpp
	src/GameBoard.hpp
	src/GameDefines.hpp
	src/GameManager.hpp
	src/GameModes.hpp
	src/GameSettings.hpp
	src/HumanPlayer.hpp
	src/InputController.hpp
	src/OutputColorizer.hpp
	src/OutputManager.hpp
	src/Player.hpp
	src/Position.hpp
)

set(SOURCE_FILES
	src/ComputerPlayer.cpp
	src/GameBoard.cpp
	src/GameManager.cpp
	src/HumanPlayer.cpp
	src/InputController.cpp
	src/main.cpp
	src/OutputManager.cpp
)

# OS specific source code
if(WIN32)
	set(PLATFORM_SOURCES
		src/platform/win/OutputColorizer.cpp
	)
else()
	set(PLATFORM_SOURCES
		src/platform/other/OutputColorizer.cpp
	)
endif()

# Source file grouping for Visual Studio
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Platform Source Files" FILES ${PLATFORM_SOURCES})

# Add all source files to executable
# Note:	Headers are not mandatory for building here.
#		They are added in order to make them visible within the VS Solution Explorer.
add_executable(${PROJECT_NAME}
	${HEADER_FILES}
	${SOURCE_FILES}
	${PLATFORM_SOURCES}
)

####################
# Build type
####################

# Set a default build type to 'RelWithDebInfo' if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message("Setting build type to 'RelWithDebInfo' as none was specified.")
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)

	# Set possible values for cmake-gui and ccmake
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

####################
# Compiler config
####################

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler specific flags
if(MSVC)
	set(COMPILER_FLAGS /W4)
else()
	set(COMPILER_FLAGS -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCE_DIR}/src
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME}
	PRIVATE ${COMPILER_FLAGS}
)

####################
# Install settings
####################

install (TARGETS ${PROJECT_NAME}
	DESTINATION bin
)
