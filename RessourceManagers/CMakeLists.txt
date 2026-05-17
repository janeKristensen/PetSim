# CMakeList.txt : CMake project for PetSim, include source and define
# project specific logic here.
#

add_library(ressource_managers STATIC 
"TextureManager.cpp" "include/TextureManager.h" 
"FontManager.cpp" "include/FontManager.h" 
"SoundManager.cpp" "include/SoundManager.h" 
"include/Animation.h" "include/AnimationManager.h" 
"AnimationManager.cpp" "Animation.cpp" 
"DialogManager.cpp" "include/DialogManager.h" 
"include/ItemManager.h" "ItemManager.cpp" 
"include/ParticleManager.h" "ParticleManager.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ressource_managers PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory ("include")

# Set the SFML path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-3.0.0)

target_include_directories(ressource_managers PUBLIC
    ${SFML_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include  
    DialogSystem/include
    entities/include
)

   # Link SFML libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ressource_managers PUBLIC
    DialogSystem
    entities
    nlohmann_json
    ${SFML_DIR}/lib/sfml-system-d.lib
    ${SFML_DIR}/lib/sfml-window-d.lib
    ${SFML_DIR}/lib/sfml-audio-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib
    )
else()
    target_link_libraries(ressource_managers PUBLIC
    DialogSystem
    entities
    nlohmann_json
    ${SFML_DIR}/lib/sfml-system.lib
    ${SFML_DIR}/lib/sfml-window.lib
    ${SFML_DIR}/lib/sfml-audio.lib
    ${SFML_DIR}/lib/sfml-graphics.lib
)
endif()

