find_package(CURL REQUIRED)

add_subdirectory(bench)
add_subdirectory(common)

target_link_libraries(bench_integration PUBLIC ${CURL_LIBRARIES})