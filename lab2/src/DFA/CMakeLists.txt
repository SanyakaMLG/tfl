project("DFA")

set(LIB_NAME libDFA)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}  ${libNode_LIB} ${libBrzozowski_LIB} ${libDerivative_LIB})
set(libDFA_LIB ${LIB_NAME})
set(libDFA_LIB ${libDFA_LIB} PARENT_SCOPE)

set(libDFA_INCLUDE_DIRS ${LIB_NAME} include)
set(libDFA_INCLUDE_DIRS ${libDFA_INCLUDE_DIRS} PARENT_SCOPE)