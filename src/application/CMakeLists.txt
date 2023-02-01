project(application_lib)

find_package(Boost 1.40 REQUIRED)

set(SOURCES
        Query.cpp BoostApplication.cpp)
set(HEADERS
        Respond.hpp QueryResponder.hpp Query.hpp BoostApplication.hpp)

add_library(application_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(application_lib PUBLIC ${Boost_LIBRARIES})