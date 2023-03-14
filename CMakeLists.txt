cmake_minimum_required(VERSION 3.18.4)
project(BladeGDK.exe)
set(CMAKE_CXX_STANDARD 17)

file(COPY core/assets DESTINATION .)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})


add_executable(${PROJECT_NAME} main.cpp program.cpp core/coreComponents.cpp core/core.cpp core/camera.cpp)

add_subdirectory(lib/glfw-3.3.8)

target_include_directories(${PROJECT_NAME} PUBLIC lib/glfw-3.3.8/include)
target_link_directories(${PROJECT_NAME} PUBLIC lib/glfw-3.3.8/src)
target_link_libraries(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME} PUBLIC lib/glm)
target_include_directories(${PROJECT_NAME} PUBLIC lib/stb_image)

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

execute_process(
    COMMAND ./compileGLSLC.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
    COMMAND ./compileGlslangValidator.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
