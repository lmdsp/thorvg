cmake_minimum_required(VERSION 3.16)

project(thorvg CXX)

# Create source groups from file list
# Used to create folder group structure in VS / XCode
# https://cmake.org/cmake/help/latest/command/source_group.html
function(tvg_assign_source_groups file_list)
    foreach(_f ${${file_list}})
        # Ensure the file path is absolute, real and doesn't contains any ".."" or ".""
        get_filename_component(_file ${_f} REALPATH)

        # Get the directory of the source file
        get_filename_component(_parent_dir "${_file}" DIRECTORY)

        # Makes _parent_dir relative to current source dir, to compute the group name
        file(RELATIVE_PATH _group ${CMAKE_CURRENT_SOURCE_DIR} ${_parent_dir})

        # Ensure Group/Subgroup hierarchy is correctly generated
        string(REPLACE "/" "\\" _group "${_group}")

        # Declare the resulting relative parent path as group name for the current file
        source_group("${_group}" FILES "${_file}")

        if(${_file} MATCHES ".*\\md")
            set_source_files_properties(${_file} PROPERTIES XCODE_EXPLICIT_FILE_TYPE "Markdown Text")
        endif()

        # message(STATUS "File ${_file} added to group ${_group}")
    endforeach()
endfunction()

if(FLUX_USE_THORVG)
    file(GLOB_RECURSE ThorVG_SRC
        src/loaders/*.cpp
        src/loaders/*.hpp
        src/renderer/*.cpp
        src/renderer/*.hpp
    )

    add_library(thorvg STATIC
        ${ThorVG_SRC}
        thorvg-unity0.cpp
        thorvg-unity1.cpp
        thorvg-unity2.cpp
        thorvg-unity3.cpp
        thorvg-unity4.cpp
        thorvg-unity5.cpp
        thorvg-unity6.cpp
        thorvg-unity7.cpp
        thorvg-unity8.cpp
        thorvg-unity9.cpp
        thorvg-unity10.cpp
        inc/config.h
        inc/thorvg.h)

    if(MSVC)
        set_property(SOURCE ${ThorVG_SRC} PROPERTY VS_SETTINGS "ExcludedFromBuild=true")
    else()
        set_source_files_properties(${ThorVG_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)
    endif()

    target_include_directories(thorvg
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/inc
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/common
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/renderer
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/renderer/sw_engine
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/loaders/svg
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/loaders/tvg
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/loaders/raw
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/loaders/lottie
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/loaders/jpg
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/loaders/png
    )

    # target_link_libraries(thorvg CONAN_PKG::nuxpixels)

    # define compile definition
    target_compile_features(thorvg PUBLIC cxx_std_17)
    target_compile_definitions(thorvg PUBLIC _UNICODE UNICODE)

    # TEMP Until HASH_KEY is fixed
    target_link_libraries(thorvg PRIVATE CONAN_PKG::xxhash)

    if(APPLE)
        target_compile_definitions(thorvg PUBLIC __PLATFORM_OSX__ __MACOS__)
        target_compile_options(thorvg PUBLIC -Wno-shorten-64-to-32)
    elseif(WIN32)
        target_compile_definitions(thorvg PUBLIC _WIN32)
        target_compile_definitions(thorvg PUBLIC NOMINMAX)
        target_compile_definitions(thorvg PUBLIC WIN32_LEAN_AND_MEAN)
        target_compile_definitions(thorvg PUBLIC TVG_STATIC)
    endif()

    # WARNING Log does not show on Windows
    # target_compile_definitions(thorvg PRIVATE $<$<CONFIG:Debug>:THORVG_LOG_ENABLED>)
    set_target_properties(thorvg PROPERTIES FOLDER ThirdParts)

    tvg_assign_source_groups(ThorVG_SRC)
endif()
