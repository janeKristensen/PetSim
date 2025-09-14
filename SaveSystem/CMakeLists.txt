# CMakeList.txt : CMake project for PetSim, include source and define
# project specific logic here.
#

add_library(save_system STATIC "Memento.cpp" "include/Memento.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET save_system PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory ("include")

target_include_directories(save_system PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    nlohmann_json
)

 target_link_libraries(save_system PUBLIC
    nlohmann_json
 )
