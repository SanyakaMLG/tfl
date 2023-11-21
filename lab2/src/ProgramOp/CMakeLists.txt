project("ProgramOp")

set(LIB_NAME libProgramOp)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)


message("SOURCES = ${SOURCES}")
message("HEADERS = ${HEADERS}")

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME}  ${libNode_LIB} )
set(libProgramOp_LIB ${LIB_NAME})
set(libProgramOp_LIB ${libProgramOp_LIB} PARENT_SCOPE)

set(libProgramOp_INCLUDE_DIRS ${LIB_NAME} include)
set(libProgramOp_INCLUDE_DIRS ${libProgramOp_INCLUDE_DIRS} PARENT_SCOPE)