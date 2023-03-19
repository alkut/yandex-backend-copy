project(application_lib)

set(SOURCES
        Query.cpp BoostApplication.cpp)
set(HEADERS
        Respond.hpp QueryResponder.hpp Query.hpp BoostApplication.hpp)

add_library(application_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(application_lib PUBLIC ${Boost_LIBRARIES})