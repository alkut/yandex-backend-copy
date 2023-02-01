project(main)

add_subdirectory(application)
add_subdirectory(model)
add_subdirectory(validation)

add_executable(main main.cpp)

target_link_libraries(application_lib PUBLIC logging_lib)
target_link_libraries(model_lib PUBLIC validation_lib view_lib)