project(repository_lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

if (NOT APPLE)
    find_library(PQXX_LIB pqxx REQUIRED)
    find_library(PQ_LIB pq REQUIRED)
    find_package(libpqxx REQUIRED)
endif()

set(SOURCES
        FileSystemRepository.cpp)
set(HEADERS
        FileSystemRepository.hpp)

add_library(repository_lib STATIC ${SOURCES} ${HEADERS})

if (NOT APPLE)
    target_link_libraries(repository_lib PUBLIC pqxx ${PQXX_LIB} ${PQ_LIB})
else()
    target_link_libraries(repository_lib PUBLIC pqxx)
endif()
