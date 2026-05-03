add_library(enet STATIC
    callbacks.c
    host.c
    list.c
    memory.c
    packet.c
    peer.c
    protocol.c
    unix.c
    win32.c
)

target_include_directories(enet PUBLIC include)

if(WIN32)
    target_compile_definitions(enet PRIVATE WIN32)
    target_link_libraries(enet PRIVATE ws2_32 winmm)
else()
    target_compile_definitions(enet PRIVATE HAS_SOCKLEN_T=1)
    target_link_libraries(enet PRIVATE m)
endif()