project(validation_lib)

find_package(Boost 1.40 REQUIRED)

set(SOURCES
        QueryExt.cpp Validator.cpp)
set(HEADERS
        DeleteItem.hpp QueryExt.hpp Validator.hpp)

add_library(validation_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(application_lib ${Boost_LIBRARIES})