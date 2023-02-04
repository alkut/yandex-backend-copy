project(model_lib)

find_package(Boost 1.40 REQUIRED)

set(SOURCES
        FileSystemTree.cpp HistoryStorage.cpp HttpServer.cpp TreeBuilder.cpp)
set(HEADERS
        FileSystemTree.hpp HistoryStorage.hpp HttpServer.hpp TreeBuilder.hpp)

add_library(model_lib STATIC ${SOURCES} ${HEADERS})

target_link_libraries(model_lib ${Boost_LIBRARIES})