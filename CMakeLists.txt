set(EXE_NAME pong)

# Target
set(SRC_FILES
    main.cpp
)

add_executable(${EXE_NAME} ${SRC_FILES})

# Link libraries
find_package(SDL REQUIRED)

include_directories(${SDL_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} ${SDL_LIBRARY})
