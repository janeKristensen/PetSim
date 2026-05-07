

add_library(particles STATIC "ParticleManager.cpp" "include/ParticleManager.h" "Particle.cpp" "include/Particle.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET particles PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory ("include")

# Set the SFML path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-3.0.0)

target_include_directories(particles PUBLIC
    ${SFML_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    
)

   # Link SFML libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(particles PUBLIC
    ${SFML_DIR}/lib/sfml-system-d.lib
    ${SFML_DIR}/lib/sfml-window-d.lib
    ${SFML_DIR}/lib/sfml-audio-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib
    )
else()
    target_link_libraries(particles PUBLIC
    ${SFML_DIR}/lib/sfml-system.lib
    ${SFML_DIR}/lib/sfml-window.lib
    ${SFML_DIR}/lib/sfml-audio.lib
    ${SFML_DIR}/lib/sfml-graphics.lib
)
endif()
