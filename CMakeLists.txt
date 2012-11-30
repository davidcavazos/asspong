cmake_minimum_required(VERSION 2.6)
project(asspong)

# Compiler flags
if (CMAKE_COMPILER_IS_GNUCXX)
#     set(CMAKE_CXX_FLAGS "-Wall -Wextra -Weffc++ -Wconversion -Wswitch-default -Wshadow -Wmissing-include-dirs")
    set(CMAKE_C_FLAGS "-Wall -Wextra -lm")
endif()

add_executable(asspong main.c)

# Link libraries
find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR})
target_link_libraries(asspong ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})
