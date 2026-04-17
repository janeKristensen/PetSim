# CMakeList.txt : CMake project for PetSim, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (PetSim "PetSim.cpp" "PetSim.h" "Pet.h" "Pet.cpp" "Item.h" "Item.cpp" "Model.h" "Model.cpp" "Main.cpp" "NeedsSystem.h" "NeedsSystem.cpp" "InventorySystem.h" "InventorySystem.cpp" "UserInterface.h" "UserInterface.cpp"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PetSim PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory ("ressources")

add_custom_command(TARGET PetSim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/bin"
        $<TARGET_FILE_DIR:PetSim>)

# Set the SFML path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-3.0.0)
set(GL_DIR ${CMAKE_SOURCE_DIR}/external/OpenGL)

include_directories(PetSim
    ${SFML_DIR}/include
    ${GL_DIR}/include
    llm_model/include
    commands/include
    save_system/include
    ressource_managers/include
    scenes/include)


target_link_libraries(PetSim PRIVATE ${OPENGL_gl_LIBRARY})


    # Link SFML libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(PetSim PUBLIC
    llm_model
    commands
    save_system
    scenes
    ressource_managers
    ${GL_DIR}/lib/glew32.lib
    ${GL_DIR}/lib/glfw3.lib
    ${GL_DIR}/lib/soil2-debug.lib
    ${SFML_DIR}/lib/sfml-system-d.lib
    ${SFML_DIR}/lib/sfml-window-d.lib
    ${SFML_DIR}/lib/sfml-audio-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib

    )
else()
    target_link_libraries(PetSim PUBLIC
    llm_model
    commands
    save_system
    scenes
    ressource_managers
    ${GL_DIR}/lib/glew32.lib
    ${GL_DIR}/lib/glfw3.lib
    ${GL_DIR}/lib/soil2-debug.lib
    ${SFML_DIR}/lib/sfml-system.lib
    ${SFML_DIR}/lib/sfml-window.lib
    ${SFML_DIR}/lib/sfml-audio.lib
    ${SFML_DIR}/lib/sfml-graphics.lib
    "D:/Visual Studio stuff/OpenGL/OpenGL template/lib/glew32.lib"
)
endif()

