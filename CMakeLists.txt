cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

if(APPLE)
        link_directories(
                /usr/local/lib
                /usr/lib
        )
        INCLUDE_DIRECTORIES(
                /usr/local/include/
                /usr/include
        )
endif()

set(dir .)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/bin")

include(FetchContent)

add_subdirectory(src)
add_subdirectory(tests)

set(LIBS
        application_lib logging_lib model_lib validation_lib view_lib)

target_link_libraries(unit_tests PUBLIC ${LIBS})
target_link_libraries(integration_tests PUBLIC ${LIBS})
target_link_libraries(bench_unit PUBLIC ${LIBS})
target_link_libraries(bench_integration PUBLIC ${LIBS})
target_link_libraries(test_util_lib PUBLIC ${LIBS})

target_link_libraries(main PUBLIC ${LIBS})