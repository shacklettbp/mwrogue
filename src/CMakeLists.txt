add_library(mwrogue_core STATIC
    game.hpp game.inl game.cpp
)

target_link_libraries(mwrogue_core PUBLIC
    madrona_core
)

add_executable(mwrogue
    main.cpp
)

target_link_libraries(mwrogue PRIVATE
    mwrogue_core
)
