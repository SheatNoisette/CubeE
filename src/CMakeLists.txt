set(CLIENT_SOURCES
    client.cpp
    clientextras.cpp
    clientgame.cpp
    clients2c.cpp
    command.cpp
    console.cpp
    editing.cpp
    entities.cpp
    main.cpp
    menus.cpp
    monster.cpp
    physics.cpp
    rendercubes.cpp
    renderextras.cpp
    rendergl.cpp
    rendermd2.cpp
    renderparticles.cpp
    rendertext.cpp
    rndmap.cpp
    savegamedemo.cpp
    server.cpp
    serverbrowser.cpp
    serverms.cpp
    serverutil.cpp
    sound.cpp
    tools.cpp
    weapon.cpp
    world.cpp
    worldio.cpp
    worldlight.cpp
    worldocull.cpp
    worldrender.cpp
)

set(SERVER_SOURCES
    serverutil.cpp
    serverms.cpp
    server.cpp
    tools.cpp
)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)

pkg_check_modules(SDL REQUIRED sdl)
pkg_check_modules(SDL_IMAGE REQUIRED SDL_image)
pkg_check_modules(SDL_MIXER REQUIRED SDL_mixer)

if(BUILD_CLIENT)
    add_executable(cube_client ${CLIENT_SOURCES})
    target_include_directories(cube_client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SDL_INCLUDE_DIRS}
        ${SDL_IMAGE_INCLUDE_DIRS}
        ${SDL_MIXER_INCLUDE_DIRS}
        ${ZLIB_INCLUDE_DIRS}
    )
    target_compile_definitions(cube_client PRIVATE
        GL_EXT_texture_env_combine
    )
    target_compile_options(cube_client PRIVATE -Wall -fsigned-char
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-c++11-narrowing -Wno-writable-strings -Wno-parentheses>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-narrowing -Wno-write-strings -Wno-parentheses>
    )
    target_link_directories(cube_client PRIVATE
        ${SDL_LIBRARY_DIRS}
        ${SDL_IMAGE_LIBRARY_DIRS}
        ${SDL_MIXER_LIBRARY_DIRS}
    )
    target_link_libraries(cube_client PRIVATE
        enet
        ${SDL_LIBRARIES}
        ${SDL_IMAGE_LIBRARIES}
        ${SDL_MIXER_LIBRARIES}
        OpenGL::GL
        OpenGL::GLU
        ${ZLIB_LIBRARIES}
    )
    if(APPLE)
        find_library(COCOA Cocoa)
        find_library(COREFOUNDATION CoreFoundation)
        find_library(APPLICATIONSERVICES ApplicationServices)
        find_library(IOKIT IOKit)
        target_link_libraries(cube_client PRIVATE ${COCOA} ${COREFOUNDATION} ${APPLICATIONSERVICES} ${IOKIT})
    endif()
endif()

if(BUILD_SERVER)
    add_executable(cube_server ${SERVER_SOURCES})
    target_include_directories(cube_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SDL_INCLUDE_DIRS}
        ${ZLIB_INCLUDE_DIRS}
    )
    target_compile_definitions(cube_server PRIVATE STANDALONE)
    target_compile_options(cube_server PRIVATE -Wall -fsigned-char
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-c++11-narrowing -Wno-writable-strings -Wno-parentheses>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-narrowing -Wno-write-strings -Wno-parentheses>
    )
    target_link_directories(cube_server PRIVATE
        ${SDL_LIBRARY_DIRS}
    )
    target_link_libraries(cube_server PRIVATE enet ${SDL_LIBRARIES} ${ZLIB_LIBRARIES})
    if(APPLE)
        find_library(COCOA Cocoa)
        target_link_libraries(cube_server PRIVATE ${COCOA})
    endif()
endif()