# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PetSim")

# Include sub-projects.
add_subdirectory ("external")
add_subdirectory ("llama.cpp")
add_subdirectory ("llm_model")
add_subdirectory ("SaveSystem")
add_subdirectory ("Commands")
add_subdirectory ("RessourceManagers")
add_subdirectory ("Scenes")
add_subdirectory ("PetSim")
add_subdirectory("ParticleSystem")
add_subdirectory("DialogSystem")
add_subdirectory("Entities")




