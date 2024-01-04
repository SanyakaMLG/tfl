project("EarleyModule")

set(LIB_NAME libEarley)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
set(libEarley_LIB ${LIB_NAME})
set(libEarley_LIB ${libEarley_LIB} PARENT_SCOPE)

set(libEarley_INCLUDE_DIRS ${LIB_NAME} include)
set(libEarley_INCLUDE_DIRS ${libEarley_INCLUDE_DIRS} PARENT_SCOPE)