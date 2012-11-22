cmake_minimum_required(VERSION 2.6)
project(asspong)

set(EXE_NAME asspong)

# Target
set(SRC_FILES
    main.cpp
)

add_executable(${EXE_NAME} ${SRC_FILES})

# Link libraries
find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})
