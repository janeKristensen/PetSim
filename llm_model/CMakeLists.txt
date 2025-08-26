# CMakeList.txt : CMake project for llm_model, include source and define
# project specific logic here.
#

# Add source to this project's executable.
#add_executable (llm_model "llm_model.cpp" "llm_model.h" )
add_library(llm_model STATIC "llm_model.cpp" "include/llm_model.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET llm_model PROPERTY CXX_STANDARD 20)
endif()
add_subdirectory ("models")
add_subdirectory ("include")

target_link_libraries(llm_model PUBLIC llama)
target_link_libraries(llm_model PUBLIC common)

target_include_directories(llm_model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)