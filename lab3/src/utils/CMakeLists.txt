project("Utils")

set(LIB_NAME libUtils)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}  ${libEarley_LIB} )
set(libUtils_LIB ${LIB_NAME})
set(libUtils_LIB ${libUtils_LIB} PARENT_SCOPE)

set(libUtils_INCLUDE_DIRS ${LIB_NAME} include)
set(libUtils_INCLUDE_DIRS ${libUtils_INCLUDE_DIRS} PARENT_SCOPE)