cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(main
        VERSION 0.0
        LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if (SANITIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-address-use-after-scope -fsanitize=undefined -fno-omit-frame-pointer -g")

    if (LEAK)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
    endif()
    if (THREAD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    endif()
endif()





set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.81 COMPONENTS unit_test_framework system program_options REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Using Boost Version ${Boost_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of Boost haven't been found")
endif()

find_package(glog 0.6.0 REQUIRED)
if (glog_FOUND)
    include_directories(${glog_INCLUDE_DIR})
    message(STATUS "Using Glog Version ${glog_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of Glog haven't been found")
endif()

find_package(Poco 1.12 REQUIRED COMPONENTS Foundation DataPostgreSQL XML JSON)
if (Poco_FOUND)
    include_directories(${Poco_INCLUDE_DIR})
    message(STATUS "Using Poco Version ${Poco_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of Poco haven't been found")
endif()

find_package(PostgreSQL 14.0 REQUIRED)
if (PostgreSQL_FOUND)
    include_directories(${PostgreSQL_INCLUDE_DIR})
    message(STATUS "Using PostgreSQL Version ${PostgreSQL_VERSION_STRING}")
else()
    message(FATAL_ERROR "Appropriate Version of PostgreSQL haven't been found")
endif()

find_package(nlohmann_json 3.11 REQUIRED)
if (nlohmann_json_FOUND)
    include_directories(${nlohmann_json_INCLUDE_DIR})
    message(STATUS "Using nlohmann_json Version ${nlohmann_json_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of nlohmann_json haven't been found")
endif()

find_package(benchmark 1.7 REQUIRED)
if (benchmark_FOUND)
    include_directories(${benchmark_INCLUDE_DIR})
    message(STATUS "Using benchmark Version ${benchmark_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of benchmark haven't been found")
endif()

find_package(CURL 7.68 REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    message(STATUS "Using CURL Version ${CURL_VERSION_STRING}")
else()
    message(FATAL_ERROR "Appropriate Version of CURL haven't been found")
endif()

find_package(GTest 1.13 REQUIRED)
if (GTest_FOUND)
    include_directories(${GTest_INCLUDE_DIR})
    message(STATUS "Using GTest Version ${GTest_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of GTest haven't been found")
endif()

find_package(libpqxx 7.7.4 REQUIRED)
if (libpqxx_FOUND)
    include_directories(${libpqxx_INCLUDE_DIR})
    message(STATUS "Using libpqxx Version ${libpqxx_VERSION}")
else()
    message(FATAL_ERROR "Appropriate Version of libpqxx haven't been found")
endif()



include_directories(src)

add_executable(main src/main.cpp)

add_subdirectory(src)

set(LIBS
        application_lib logging_lib model_lib validation_lib view_lib)


target_link_libraries(main PUBLIC ${LIBS})




if (ENABLE_TEST)
    enable_testing()
    add_subdirectory(tests)

    target_link_libraries(unit_tests PUBLIC ${LIBS})
    target_link_libraries(integration_tests PUBLIC ${LIBS})
    target_link_libraries(bench_unit PUBLIC ${LIBS})
    target_link_libraries(bench_integration PUBLIC ${LIBS})
    target_link_libraries(test_util_lib PUBLIC ${LIBS})
endif()