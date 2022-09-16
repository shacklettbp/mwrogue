set(MADROGUE_CORE_SRCS
    game.hpp game.inl game.cpp
)

add_library(madrogue_sw_core STATIC
    ${MADROGUE_CORE_SRCS}
)
target_link_libraries(madrogue_sw_core PUBLIC
    madrona_core
)

add_executable(madrogue
    main.cpp
)
target_link_libraries(madrogue PRIVATE
    madrogue_sw_core
)

add_library(madrogue_mw_core STATIC
    ${MADROGUE_CORE_SRCS}
)
target_link_libraries(madrogue_mw_core PUBLIC
    madrona_mw_core
)

add_executable(madrogue_mw
    mw_cpu.cpp
)
target_link_libraries(madrogue_mw PRIVATE
    madrogue_mw_core
)
