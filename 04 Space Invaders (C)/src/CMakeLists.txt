cmake_minimum_required (VERSION 3.5)

# First arg = path to library
# Second arg = path to build
add_subdirectory(intc intc)
add_subdirectory(btns btns)
add_subdirectory(fit fit)
add_subdirectory(switches switches)
add_subdirectory(image_render image_render)
add_subdirectory(video video)
add_subdirectory(tank tank)
add_subdirectory(saucer saucer)
add_subdirectory(globals globals)
add_subdirectory(aliens aliens)
add_subdirectory(control control)
add_subdirectory(tank_bullet tank_bullet)
add_subdirectory(alien_bullet alien_bullet)
add_subdirectory(bunkers bunkers)
add_subdirectory(user_audio user_audio)

# Path to header files
include_directories(intc)
include_directories(btns)
include_directories(fit)
include_directories(switches)
include_directories(image_render)
include_directories(video)
include_directories(tank)
include_directories(saucer)
include_directories(globals)
include_directories(aliens)
include_directories(control)
include_directories(tank_bullet)
include_directories(alien_bullet)
include_directories(bunkers)
include_directories(user_audio)

add_executable(lab6_main main.c)

# First arg = executable to link it to
# Second arg = where to find the library
target_link_libraries(lab6_main control)
target_link_libraries(lab6_main tank_bullet)
target_link_libraries(lab6_main user_audio)
target_link_libraries(lab6_main aliens)
target_link_libraries(lab6_main alien_bullet)
target_link_libraries(lab6_main saucer)
target_link_libraries(lab6_main tank)
target_link_libraries(lab6_main bunkers)
target_link_libraries(lab6_main image_render)
target_link_libraries(lab6_main video)
target_link_libraries(lab6_main btns)
target_link_libraries(lab6_main fit)
target_link_libraries(lab6_main switches)
target_link_libraries(lab6_main intc)
target_link_libraries(lab6_main globals)


