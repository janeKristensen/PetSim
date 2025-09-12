# CMakeList.txt : CMake project for PetSim, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (PetSim "PetSim.cpp" "PetSim.h" "Pet.h" "Pet.cpp" "Item.h" "Item.cpp" "Scene.h"  "Scene.cpp" "Model.h" "Model.cpp" "Main.cpp" "NeedsSystem.h" "NeedsSystem.cpp" "InventorySystem.h" "InventorySystem.cpp" "UserInterface.h" "UserInterface.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PetSim PROPERTY CXX_STANDARD 20)
endif()

# Set the SFML path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-3.0.0)

include_directories(PetSim
    ${SFML_DIR}/include
    llm_model/include)

    # Link SFML libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(PetSim PUBLIC
    llm_model
    ${SFML_DIR}/lib/sfml-system-d.lib
    ${SFML_DIR}/lib/sfml-window-d.lib
    ${SFML_DIR}/lib/sfml-audio-d.lib
    ${SFML_DIR}/lib/sfml-graphics-d.lib
    )
else()
    target_link_libraries(PetSim PUBLIC
    llm_model
    ${SFML_DIR}/lib/sfml-system.lib
    ${SFML_DIR}/lib/sfml-window.lib
    ${SFML_DIR}/lib/sfml-audio.lib
    ${SFML_DIR}/lib/sfml-graphics.lib
)
endif()