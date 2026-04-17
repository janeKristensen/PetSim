# CMakeList.txt : CMake project for PetSim, include source and define
# project specific logic here.
#

add_library(scenes STATIC "scene.cpp" "include/scene.h" "GameScene.cpp" "include/GameScene.h"  "SceneManager.cpp" "include/SceneManager.h" "include/MenuScene.h" "MenuScene.cpp" "include/TitleScene.h" "TitleScene.cpp" "LoadingScene.cpp" "include/LoadingScene.h" "include/ShopScene.h" "ShopScene.cpp" "include/LitterScene.h" "LitterScene.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET scenes PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory ("include")

# Set the SFML path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-3.0.0)
set(GL_DIR ${CMAKE_SOURCE_DIR}/external/OpenGL)

target_include_directories(scenes PUBLIC
    ${CMAKE_SOURCE_DIR}/PetSim
    ${GL_DIR}/include
    ${SFML_DIR}/include
    llm_model/include
    commands/include
    particles/include
    save_system/include
    ressource_managers/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include   
)

   # Link SFML libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(scenes PUBLIC
    PetSim
    llm_model
    commands
    particles
    save_system
    ressource_managers
    ${GL_DIR}/lib/glew32.lib
    ${GL_DIR}/lib/glfw3.lib
    ${GL_DIR}/lib/soil2-debug.lib
    ${SFML_DIR}/lib/sfml-system-d.lib
    ${SFML_DIR}/lib/sfml-window-d.lib
    ${SFML_DIR}/lib/sfml-audio-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib
    )
else()
    target_link_libraries(scenes PUBLIC
    PetSim
    llm_model
    commands
    particles
    save_system
    ressource_managers
    ${GL_DIR}/lib/glew32.lib
    ${GL_DIR}/lib/glfw3.lib
    ${GL_DIR}/lib/soil2-debug.lib
    ${SFML_DIR}/lib/sfml-system.lib
    ${SFML_DIR}/lib/sfml-window.lib
    ${SFML_DIR}/lib/sfml-audio.lib
    ${SFML_DIR}/lib/sfml-graphics.lib
)
endif()
