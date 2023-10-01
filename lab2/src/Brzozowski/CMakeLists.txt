project("Brzozowski")

set(LIB_NAME libBrzozowski)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}  ${libNode_LIB} )
target_link_libraries(${LIB_NAME}  ${libDerivative_LIB} )
set(libBrzozowski_LIB ${LIB_NAME})
set(libBrzozowski_LIB ${libBrzozowski_LIB} PARENT_SCOPE)

set(libBrzozowski_INCLUDE_DIRS ${LIB_NAME} include)
set(libBrzozowski_INCLUDE_DIRS ${libBrzozowski_INCLUDE_DIRS} PARENT_SCOPE)

