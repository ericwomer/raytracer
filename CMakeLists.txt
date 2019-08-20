cmake_minimum_required(VERSION 3.15)
project(raytrace VERSION 0.0.1.0)

option(with_system_tbb
        "Use systems tbb install" ON)
option(build_docs
        "Build docs" OFF)
option(onative
        "Optimize for the host CPU" OFF)
option(winrt
        "Build for Windows RT" OFF)
option(use_intrinsics
        "Compile using intrinsics (e.g mmx, sse, msse2)" ON) # Rake: is this needed?

set(cpu_type "" CACHE STRING "When set, passes this string as CPU-ID which will be embedded into the binary.")

set(cpu_optimizations "-mmmx -msse -msse2" CACHE STRING "Which CPU specific optimitations should be used beside the compiler's default?")



if(MSVC)
    #message(STATUS CMAKE_ROOT: ${CMAKE_ROOT})

    #if(CMAKE_CL_64)
    #	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin/win64)
    #else()
    #	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin/win32)
    #endif()

    #message(STATUS EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH})
    #message(STATUS PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR})
    #message(STATUS CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR})
else()
    message(STATUS CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_definitions(-pipe)
    #add_definitions(-Wall)
    add_definitions(-Werror=format-security)
    add_definitions(-Werror=format)
    if(cpu_type)
        add_definitions(-DCPUSTRING="${cpu_type}")
    endif()
    if (cpu_optimization)
        add_definitions(${cpu_optimization})
    endif()
    if (use_intrinsics)
        add_definitions(-DUSE_INTRINSICS)
    endif()
    if(WIN32)
        # require msvcr70.dll or newer for _aligned_malloc etc
        # I think it is from Visual C++ .NET 2002, so it should be available on any remotely modern system.
        add_definitions(-D__MSVCRT_VERSION__=0x0700)
    endif()

    #if(NOT ANDROID)
    add_definitions(-DUSE_EXCEPTIONS)
    #endif()

    add_compile_options(-Wno-pragmas -Wno-unused-variable -Wno-switch -Wno-unused-value -Winvalid-pch -Wno-multichar)
    if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
        # add clang-specific settings for warnings (the second one make sure clang doesn't complain
        # about unknown -W flags, like -Wno-unused-but-set-variable)
        add_compile_options(-Wno-local-type-template-args -Wno-unknown-warning-option -Wno-inline-new-delete -Wno-switch-enum)
    endif()

    if(NOT CMAKE_CROSSCOMPILING AND onative)
        add_definitions(-march=native)
    endif()

    add_compile_options(-fno-strict-aliasing)

    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG -O0 -ggdb")
    #set(CMAKE_C_FLAGS_DEBUGALL "${CMAKE_C_FLAGS_DEBUGALL} -g -ggdb -D_DEBUG")
    #set(CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS_PROFILE} -g -ggdb -D_DEBUG -O1 -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -O3 -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Os -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer")

    set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
    #set(CMAKE_CXX_FLAGS_DEBUGALL ${CMAKE_C_FLAGS_DEBUGALL})
    #set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_C_FLAGS_PROFILE})
    set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    set(CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL})

elseif(MSVC)

    # Multiprocessor support for compiling
    # /MP

    # /MT  use multithreaded, static runtime library
    #      (=> binary won't need MSVCP100D.dll or similar)
    # /MTd the same with debug symbols
    # /MD and /MDd - the same with dynamic runtime lib
    #     (=> MSVCP100D.dll or similar is needed to run binary)

    # Inline Function Expansion: Any Suitable (/Ob2)
    # "/Ob2",

    # enable Intrinsic Functions
    # "/Oi",

    # Omit Frame Pointers
    # "/Oy",

    # if(MSVC_VERSION EQUAL 1700)
    # #message(STATUS "$ENV{LIBPATH}")

    # find_path(Windows_winmd_DIR NAMES Windows.winmd
    # HINTS
    # C:/Program Files (x86)/Windows Kits/8.0/References/CommonConfiguration/Neutral
    # PATH_SUFFIXES "Neutral")

    # message(STATUS "${Windows_winmd_DIR}")

    # #set(WINRT_OPTIONS "/ZW /D USE_WINRT /AI \"C:\Program Files (x86)\Windows Kits\8.0\References\CommonConfiguration\Neutral\"")
    # #set(WINRT_OPTIONS "/ZW /D USE_WINRT /AI \"$ENV{LIBPATH}\" ")
    # set(WINRT_OPTIONS "/ZW /D USE_WINRT")

    # set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP ${WINRT_OPTIONS}")
    # set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy ${WINRT_OPTIONS}")
    # set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MP /Oi ${WINRT_OPTIONS}")
    # set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MP /Oi /Oy ${WINRT_OPTIONS}")

    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP ${WINRT_OPTIONS}")
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy ${WINRT_OPTIONS}")
    # set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP ${WINRT_OPTIONS}")
    # set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /Oi /Oy ${WINRT_OPTIONS}")
    # else()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy /MT")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MP /Oi /MTd")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MP /Oi /Oy /MT")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy /MT")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP /MTd")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /Oi /Oy /MT")

    # RB: without /SAFESEH:NO we can't link against ffmpeg libs and VS2013 or we have to build our own
    # libs for newer compilers
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /SAFESEH:NO")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SAFESEH:NO")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /SAFESEH:NO")
    # endif()


    add_definitions(-DWIN64
            -D_WINDOWS
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_WARNINGS
            -D_MBCS
            #-DUSE_OPENAL
            -DUSE_EXCEPTIONS)
    ## Check for Version ##
    if( winrt OR WINDOWS10 ) # Windows RT
        add_definitions(-DUSE_WINRT)
    endif()

    if (use_intrinsics) # Rake: Is this needed?
        add_definitions(-DUSE_INTRINSICS)
    endif()
endif()

if(MSVC)
    include_directories ("${PROJECT_SOURCE_DIR}/src")
    include_directories ("${PROJECT_SOURCE_DIR}/thirdparty/include")
    include_directories ("${PROJECT_SOURCE_DIR}/thirdparty/tbb2019_20190605oss/include")
    include_directories ("${PROJECT_BINARY_DIR}")

    add_compile_options("-std:c++17")

    add_definitions("-D_USE_MATH_DEFINES")
    add_definitions("-DNOMINMAX")

else()
    include(FindPkgConfig)
    find_package(PkgConfig REQUIRED)

    # Hard code to local built version?
    # make compiler=clang stdlib=libc++ stdver=c++17 tbb_build_prefix=lib
    if(with_system_tbb)
        message(STATUS "Using system installed tbb")
        pkg_check_modules(tbb REQUIRED tbb)
        include_directories("${tbb_INCLUDEDIR}")
    else() # Rake: Maybe add an option to build this if its not already built.
        message(STATUS "Using locally built tbb")
        find_library(TBB tbb PATHS "${PROJECT_SOURCE_DIR}/src/thirdparty/tbb/build/lib_release" NO_DEFAULT_PATH)
        if(TBB)
            message(STATUS "tbb found!")
            message(STATUS "Using local tbb for threading.")
            message(STATUS ${TBB})
            add_definitions("-DUSING_TBB")
            include_directories("${PROJECT_SOURCE_DIR}/src/thirdparty/tbb/include")
        elseif()
            message(WARNING "tbb not found! Threading is disabled for this project.")
        endif()
    endif()

    # Inforce use of clang++
    set(CMAKE_CXX_COMPILER "clang++")

    include_directories ("${PROJECT_SOURCE_DIR}/src")
    include_directories ("${PROJECT_SOURCE_DIR}/thirdparty/include")
    include_directories ("${PROJECT_BINARY_DIR}")

    include_directories("/usr/lib/llvm-8/include/c++/v1/") # Rake: I don't like this hard coded here, find a way to find it, maybe pkg-config?
    add_compile_options("-std=c++17")
    add_compile_options("-stdlib=libc++")

    link_libraries("c++")
    link_libraries("c++abi")
    link_libraries("m")
endif()


# build configuration header file
set(name ${CMAKE_PROJECT_NAME})

set(compiler_name ${CMAKE_CXX_COMPILER_ID})
set(compiler_version ${CMAKE_CXX_COMPILER_VERSION})
set(host_machine ${CMAKE_HOST_SYSTEM_NAME})
set(cpu_family ${CMAKE_HOST_SYSTEM_PROCESSOR})

set(major ${PROJECT_VERSION_MAJOR})
set(minor ${PROJECT_VERSION_MINOR})
set(patch ${PROJECT_VERSION_PATCH})
set(compile ${PROJECT_VERSION_TWEAK})

# configure header file
configure_file(
        ${PROJECT_SOURCE_DIR}/src/config.h.in
        ${PROJECT_BINARY_DIR}/config.h
)

set (raytrace_src
        "src/scene/scene.cpp"
        "src/rendering/raytracer.cpp"
        "src/system/system.cpp"
        "src/system/file.cpp"
        "src/objects/sphere.cpp"
        "src/objects/hitable.cpp"
        "src/main.cpp"
        "src/raytracerapp.cpp"
        "src/objects/moving_sphere.cpp"
        "src/objects/moving_sphere.h"
        )

if(MSVC)
    link_directories("${CMAKE_SOURCE_DIR}/thirdparty/tbb2019_20190605oss/lib/intel64/vc14")
    add_executable(raytrace ${raytrace_src})
    target_link_libraries(raytrace tbb_debug)
else()
    link_directories("${CMAKE_SOURCE_DIR}/src/thridparty/tbb/build/lib_release") # Is this needed?
    add_executable(raytrace ${raytrace_src})
    target_link_libraries(raytrace ${TBB})
endif()

if(build_docs)
    add_subdirectory(doc)
endif()

#[[
if(MSVC)

    # if(MSVC_VERSION EQUAL 1700)
    # set(DirectX_LIBRARIES
    # dinput8
    # dsound
    # dxguid
    # #dxerr
    # Xinput
    # xaudio2.lib
    # )

    # if(NOT OPENAL)
    # list(APPEND DirectX_LIBRARIES xaudio2.lib)
    # endif()

    # else()

    if(WINDOWS10)
        set(DirectX_LIBRARIES
                dinput8
                dsound
                dxguid
                #dxerr
                Xinput
                xaudio2.lib
                )
    else()
        find_package(DirectX REQUIRED)
        include_directories(${DirectX_INCLUDE_DIR})
    endif()

    if(OPENAL)
        add_definitions(-DUSE_OPENAL)

        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/include)

        if(CMAKE_CL_64)
            link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/lib/win64)
        else()
            link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/lib/win32)
        endif()

        list(APPEND RBDOOM3_INCLUDES ${OPENAL_INCLUDES})
        list(APPEND RBDOOM3_SOURCES
                ${OPENAL_SOURCES}
                ${DOOMCLASSIC_OPENAL_SOURCES})

        set(OpenAL_LIBRARIES
                OpenAL32)

        if(CMAKE_CL_64)
            install(FILES libs/openal-soft/lib/win64/OpenAL64.dll DESTINATION .)
        else()
            install(FILES libs/openal-soft/lib/win32/OpenAL32.dll DESTINATION .)
            install(FILES libs/openal-soft/lib/win32/OpenAL32.pdb DESTINATION .)
        endif()
    else()
        list(APPEND RBDOOM3_INCLUDES ${XAUDIO2_INCLUDES})
        list(APPEND RBDOOM3_SOURCES
                ${XAUDIO2_SOURCES}
                ${DOOMCLASSIC_XAUDIO2_SOURCES})
    endif()

    if(FFMPEG)
        add_definitions(-DUSE_FFMPEG)

        if(CMAKE_CL_64)
            include_directories(libs/ffmpeg-win64/include)
            include_directories(libs/ffmpeg-win64/include/libswscale)
            include_directories(libs/ffmpeg-win64/include/libavformat)
            include_directories(libs/ffmpeg-win64/include/libavdevice)
            include_directories(libs/ffmpeg-win64/include/libavcodec)

            link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/ffmpeg-win64/lib)
        else()
            include_directories(libs/ffmpeg-win32/include)
            include_directories(libs/ffmpeg-win32/include/libswscale)
            include_directories(libs/ffmpeg-win32/include/libavformat)
            include_directories(libs/ffmpeg-win32/include/libavdevice)
            include_directories(libs/ffmpeg-win32/include/libavcodec)

            link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/ffmpeg-win32/lib)
        endif()

        set(FFmpeg_LIBRARIES
                avcodec
                avformat
                avutil
                swscale)

    endif()

    if(USE_MFC_TOOLS)

        include_directories(libs/atlmfc/include)
        if(CMAKE_CL_64)
            link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/atlmfc/lib/amd64)
        else()
            link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/atlmfc/lib)
        endif()

        add_definitions(-DUSE_MFC_TOOLS)

        list(APPEND RBDOOM3_SOURCES
                ${TOOLS_INCLUDES}
                ${TOOLS_COMAFX_INCLUDES} ${TOOLS_COMAFX_SOURCES}
                ${TOOLS_COMMON_INCLUDES} ${TOOLS_COMMON_SOURCES}
                ${TOOLS_COMMON_PROPTREE_INCLUDES} ${TOOLS_COMMON_PROPTREE_SOURCES}

                ${EDITOR_AF_INCLUDES} ${EDITOR_AF_SOURCES}
                ${EDITOR_DEBUGGER_INCLUDES} ${EDITOR_DEBUGGER_SOURCES}
                ${EDITOR_DECL_INCLUDES} ${EDITOR_DECL_SOURCES}
                ${EDITOR_GUIED_INCLUDES} ${EDITOR_GUIED_SOURCES}
                ${EDITOR_MATERIALEDITOR_INCLUDES} ${EDITOR_MATERIALEDITOR_SOURCES}
                ${EDITOR_PARTICLE_INCLUDES} ${EDITOR_PARTICLE_SOURCES}
                ${EDITOR_PDA_INCLUDES} ${EDITOR_PDA_SOURCES}
                ${EDITOR_RADIANT_INCLUDES} ${EDITOR_RADIANT_SOURCES}
                ${EDITOR_SCRIPT_INCLUDES} ${EDITOR_SCRIPT_SOURCES}
                ${EDITOR_SOUND_INCLUDES} ${EDITOR_SOUND_SOURCES})
    endif()

    list(APPEND RBDOOM3_INCLUDES
            ${SYS_INCLUDES}
            ${WIN32_INCLUDES})

    list(APPEND RBDOOM3_SOURCES
            ${SYS_SOURCES}
            ${WIN32_SOURCES})

    list(REMOVE_DUPLICATES RBDOOM3_SOURCES)

    if(USE_PRECOMPILED_HEADERS)
        set(RBDOOM3_PRECOMPILED_SOURCES ${RBDOOM3_SOURCES})
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${TIMIDITY_SOURCES} ${JPEG_SOURCES} ${PNG_SOURCES} ${ZLIB_SOURCES} ${GLEW_SOURCES} ${BINKDEC_SOURCES})
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/minizip/ioapi.c)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/framework/precompiled.cpp)


        set_source_files_properties(
                ${RBDOOM3_PRECOMPILED_SOURCES}
                PROPERTIES
                COMPILE_FLAGS "/Yuprecompiled.h"
                OBJECT_DEPENDS "precompiled.pch"

        )

        set_source_files_properties(framework/precompiled.cpp
                PROPERTIES
                COMPILE_FLAGS "/Ycprecompiled.h"
                OBJECT_OUTPUTS "precompiled.pch"
                )
    endif()

    list(APPEND RBDOOM3_SOURCES ${WIN32_RESOURCES})

    add_executable(RBDoom3BFG WIN32 ${RBDOOM3_INCLUDES} ${RBDOOM3_SOURCES})

    #add_custom_command(TARGET RBDoom3BFG
    #               PRE_BUILD
    #			   COMMAND nmake ARGS /f Makefile.vc6 CFG=release
    #			   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/curl/lib
    #               COMMENT "Compiling libcURL")

    add_dependencies(RBDoom3BFG idlib)
    target_link_libraries(RBDoom3BFG
            idlib
            ${DirectX_LIBRARIES}
            #libcurl
            #openal32
            opengl32
            glu32
            dbghelp
            #eaxguid
            iphlpapi
            winmm
            wsock32.lib
            ${OpenAL_LIBRARIES}
            ${FFmpeg_LIBRARIES}
            ${ZLIB_LIBRARY}
            ${PNG_LIBRARY}
            ${JPEG_LIBRARY}
            ${GLEW_LIBRARY}
            ${CMAKE_DL_LIBS}
            )

    #CMAKE_BINARY_DIR
    if(CMAKE_CL_64)
        install(TARGETS RBDoom3BFG
                RUNTIME DESTINATION .)
    else()
        install(TARGETS RBDoom3BFG
                RUNTIME DESTINATION .)
    endif()

else()

    list(APPEND RBDOOM3_SOURCES
            ${SYS_INCLUDES} ${SYS_SOURCES})

    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})

    if(UNIX)
        if(FFMPEG)
            find_package(FFMPEG REQUIRED)
            add_definitions(-DUSE_FFMPEG)

            include_directories(${FFMPEG_INCLUDE_DIR})
            link_directories(${FFMPEG_LIBRARIES_DIRS})
        endif()

        if(SDL2)
            find_package(SDL2 REQUIRED)
            include_directories(${SDL2_INCLUDE_DIR})
            set(SDLx_LIBRARY ${SDL2_LIBRARY})
        else()
            find_package(SDL REQUIRED)
            include_directories(${SDL_INCLUDE_DIR})
            set(SDLx_LIBRARY ${SDL_LIBRARY})
        endif()

        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            list(REMOVE_ITEM POSIX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/posix/platform_linux.cpp)
        else()
            list(REMOVE_ITEM POSIX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/posix/platform_osx.cpp)
        endif()

        list(APPEND RBDOOM3_SOURCES
                ${POSIX_INCLUDES} ${POSIX_SOURCES}
                ${SDL_INCLUDES} ${SDL_SOURCES})

        if(OPENAL)
            find_package(OpenAL REQUIRED)
            add_definitions(-DUSE_OPENAL)

            #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/include)

            #if(CMAKE_CL_64)
            #	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/lib/win64)
            #else()
            #	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/lib/win32)
            #endif()

            list(APPEND RBDOOM3_INCLUDES ${OPENAL_INCLUDES})
            list(APPEND RBDOOM3_SOURCES
                    ${OPENAL_SOURCES}
                    ${DOOMCLASSIC_OPENAL_SOURCES})
        else()
            list(APPEND RBDOOM3_INCLUDES ${STUBAUDIO_INCLUDES})
            list(APPEND RBDOOM3_SOURCES
                    ${STUBAUDIO_SOURCES}
                    ${DOOMCLASSIC_STUBAUDIO_SOURCES})
        endif()

    endif()

    list(REMOVE_DUPLICATES RBDOOM3_SOURCES)

    if(USE_PRECOMPILED_HEADERS)
        set(RBDOOM3_PRECOMPILED_SOURCES ${RBDOOM3_SOURCES})
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${TIMIDITY_SOURCES} ${JPEG_SOURCES} ${PNG_SOURCES} ${ZLIB_SOURCES} ${GLEW_SOURCES} ${BINKDEC_SOURCES})
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/minizip/ioapi.c)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp)
        list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp)

        foreach( src_file ${RBDOOM3_PRECOMPILED_SOURCES} )
            #message(STATUS "-include precompiled.h for ${src_file}")
            set_source_files_properties(
                    ${src_file}
                    PROPERTIES
                    COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/idlib/precompiled.h"
            )
        endforeach()

        # precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
        STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
        SET(_compiler_FLAGS ${${_flags_var_name}})
        GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
        FOREACH(item ${_directory_flags})
            LIST(APPEND _compiler_FLAGS " -I${item}")
        ENDFOREACH(item)
    endif()

    GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
    LIST(APPEND _compiler_FLAGS ${_directory_flags})
    SEPARATE_ARGUMENTS(_compiler_FLAGS)

    if(USE_PRECOMPILED_HEADERS)
        # we need to recreate the precompiled header for RBDoom3BFG
        # (i.e. can't use the one created for idlib before)
        # because some definitions (e.g. -D__IDLIB__ -D__DOOM_DLL__) differ
        add_custom_target(precomp_header_rbdoom3bfg ALL
                COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header idlib/precompiled.h -o idlib/precompiled.h.gch
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Creating idlib/precompiled.h.gch for RBDoom3BFG"
                )
    endif()

    if(WIN32)
        set(remove_command "del")
    else()
        set(remove_command "rm")
    endif()

    if (USE_PRECOMPILED_HEADERS)
        # it's ugly enough that the precompiled header binary needs to be in the
        # source directory (instead of the build directory), so let's at least
        # delete it after build.
        add_custom_target(rm_precomp_header ALL
                COMMAND ${remove_command} "idlib/precompiled.h.gch"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "remove idlib/precompiled.h.gch"
                )

        # make sure this is run after creating idlib
        add_dependencies(precomp_header_rbdoom3bfg idlib)
    endif()

    add_executable(RBDoom3BFG WIN32 ${RBDOOM3_SOURCES})

    if (USE_PRECOMPILED_HEADERS)
        # make sure precompiled header is created before executable is compiled
        add_dependencies(RBDoom3BFG precomp_header_rbdoom3bfg)

        # make sure precompiled header is deleted after executable is compiled
        add_dependencies(rm_precomp_header RBDoom3BFG)
    endif()

    if(NOT WIN32)
        if(NOT "${CMAKE_SYSTEM}" MATCHES "Darwin")
            set(RT_LIBRARY rt)
        endif()

        target_link_libraries(RBDoom3BFG
                idlib
                ${OPENGL_LIBRARIES}
                pthread
                ${RT_LIBRARY}
                ${SDLx_LIBRARY}
                ${OPENAL_LIBRARY}
                ${FFMPEG_LIBRARIES}
                ${ZLIB_LIBRARY}
                ${PNG_LIBRARY}
                ${JPEG_LIBRARY}
                ${GLEW_LIBRARY}
                ${CMAKE_DL_LIBS}
                )
    endif()

endif()
]]
#[[
project(
        'Ray Tracing App',
        'cpp',
        version : '0.0.1',
        default_options:
        [
        'cpp_std=c++17',
        'b_lundef=false',
        'b_sanitize=none',
        'warning_level=3'
        ],
        meson_version: '>=0.48.0'
)

pkgconfig = import('pkgconfig')

build_number_sh = find_program('build_number.sh', 'tools/build_number.sh')
build_number_increment = run_command(build_number_sh)
build_number = run_command('cat', files('build_number.txt'),).stdout().strip()

message(build_number)

cpp = meson.get_compiler('cpp')

conf_data = configuration_data()
conf_data.set('name', meson.project_name())

conf_data.set('compiler_name', cpp.get_id())
conf_data.set('compiler_version', cpp.version())
conf_data.set('host_machine', host_machine.system())
conf_data.set('cpu_family', build_machine.cpu_family())

conf_data.set('version_number', meson.project_version() + '.' + build_number)

configure_file(input : 'src/config.h.in',
output : 'config.h',
configuration : conf_data)

raytrace_src = [
'src/scene/scene.cpp',
'src/rendering/raytracer.cpp',
'src/system/system.cpp',
'src/system/file.cpp',
'src/objects/sphere.cpp',
'src/objects/hitable.cpp',
'src/main.cpp',
'src/raytracerapp.cpp'
]

raytrace_include_directories = [
include_directories('src/'),
include_directories('thirdparty/include/'),
include_directories('/usr/lib/llvm-8/include/c++/v1/')
]

raytrace_cflags = ['-stdlib=libc++']
raytrace_ldflags = [
'-lm',
'-lc++',
'-lc++abi']

threads = dependency('threads')

raytrace_deps = [threads]

if not get_option('with_system_tbb')
tbb_prefix = 'thirdparty/tbb/'

# tbb build command, replace clang with cpp's name and the stdver and
# make compiler=clang stdlib=libc++ stdver=c++17 tbb_build_prefix=lib

if get_option('buildtype') == 'debug'
libtbb = cpp.find_library(
'tbb_debug',
dirs : join_paths(meson.source_root(), tbb_prefix, 'build/lib_debug')
)
tbb_link_path = '-L' + meson.source_root() + '/' + tbb_prefix + 'build/lib_debug'
else
libtbb = cpp.find_library(
'tbb',
dirs : join_paths(meson.source_root(), tbb_prefix, 'build/lib_release')
)
tbb_link_path = '-L' + meson.source_root() + '/' + tbb_prefix + 'build/lib_release'
endif

raytrace_deps += declare_dependency(
include_directories: include_directories(tbb_prefix + '/include'),
dependencies: [libtbb],
link_args: ['-latomic', tbb_link_path]
)

endif

raytrace = executable(
'raytracer',
raytrace_src,
dependencies: raytrace_deps,
include_directories: raytrace_include_directories,
cpp_args: raytrace_cflags,
link_args: raytrace_ldflags
)
]]