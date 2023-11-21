project("Node")

set(LIB_NAME libNode)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)


message("SOURCES = ${SOURCES}")
message("HEADERS = ${HEADERS}")

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)

set(libNode_LIB ${LIB_NAME})
set(libNode_LIB ${libNode_LIB} PARENT_SCOPE)

set(libNode_INCLUDE_DIRS ${LIB_NAME} include)
set(libNode_INCLUDE_DIRS ${libNode_INCLUDE_DIRS} PARENT_SCOPE)