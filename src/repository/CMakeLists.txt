project(repository_lib)

set(SOURCES
        FileSystemRepository.cpp)
set(HEADERS
        FileSystemRepository.hpp)

add_library(repository_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(repository_lib PUBLIC pqxx pq)