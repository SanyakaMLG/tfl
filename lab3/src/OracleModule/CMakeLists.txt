project("OracleModule")

set(LIB_NAME libOracle)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}  ${libUtils_LIB} )
set(libOracle_LIB ${LIB_NAME})
set(libOracle_LIB ${libOracle_LIB} PARENT_SCOPE)

set(libOracle_INCLUDE_DIRS ${LIB_NAME} include)
set(libOracle_INCLUDE_DIRS ${libOracle_INCLUDE_DIRS} PARENT_SCOPE)