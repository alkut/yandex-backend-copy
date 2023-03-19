project(validation_lib)

set(SOURCES
        QueryExt.cpp Validator.cpp)
set(HEADERS
        DeleteItem.hpp QueryExt.hpp Validator.hpp)

add_library(validation_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(application_lib PUBLIC Boost::system)