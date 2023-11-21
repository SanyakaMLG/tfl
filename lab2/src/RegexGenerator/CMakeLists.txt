project("RegexGenerator")

set(LIB_NAME libRegexGenerator)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)


message("SOURCES = ${SOURCES}")
message("HEADERS = ${HEADERS}")

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}   ${libNode_LIB} ${libDerivative_LIB})
set(libRegexGenerator_LIB ${LIB_NAME})
set(libRegexGenerator_LIB ${libRegexGenerator_LIB} PARENT_SCOPE)

set(libRegexGenerator_INCLUDE_DIRS ${LIB_NAME} include)
set(libRegexGenerator_INCLUDE_DIRS ${libRegexGenerator_INCLUDE_DIRS} PARENT_SCOPE)