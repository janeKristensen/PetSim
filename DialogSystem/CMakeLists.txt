

add_library (DialogSystem STATIC "DialogSystem.cpp" "include/DialogSystem.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DialogSystem PROPERTY CXX_STANDARD 20)
endif()



target_include_directories(DialogSystem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    nlohmann_json
)

 target_link_libraries(DialogSystem PUBLIC
    nlohmann_json
 )
