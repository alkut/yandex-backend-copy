project(global_lib)

set(SOURCES
        ProgramOptions.cpp)
set(HEADERS
        ProgramOptions.h)

add_library(global_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(global_lib PUBLIC Boost::program_options)