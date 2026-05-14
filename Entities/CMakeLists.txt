

add_library(entities STATIC "pet.cpp" "include/pet.h" "item.cpp" "include/item.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET entities PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory("include")

# Set the SFML path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-3.0.0)

target_include_directories(entities PUBLIC
    ${SFML_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    save_system/include
    ressource_managers/include
)

   # Link SFML libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(entities PUBLIC
    save_system
    ressource_managers
    ${SFML_DIR}/lib/sfml-system-d.lib
    ${SFML_DIR}/lib/sfml-window-d.lib
    ${SFML_DIR}/lib/sfml-audio-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib
    )
else()
    target_link_libraries(entities PUBLIC
    save_system
    ressource_managers
    ${SFML_DIR}/lib/sfml-system.lib
    ${SFML_DIR}/lib/sfml-window.lib
    ${SFML_DIR}/lib/sfml-audio.lib
    ${SFML_DIR}/lib/sfml-graphics.lib
)
endif()
