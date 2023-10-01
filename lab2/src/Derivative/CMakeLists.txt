project("Derivative")

set(LIB_NAME libDerivative)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)


message("SOURCES = ${SOURCES}")
message("HEADERS = ${HEADERS}")

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}  ${libNode_LIB} )
set(libDerivative_LIB ${LIB_NAME})
set(libDerivative_LIB ${libDerivative_LIB} PARENT_SCOPE)

set(libDerivative_INCLUDE_DIRS ${LIB_NAME} include)
set(libDerivative_INCLUDE_DIRS ${libDerivative_INCLUDE_DIRS} PARENT_SCOPE)