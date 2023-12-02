set(PROJECT_NAME "lab3")

project(${PROJECT_NAME})
add_executable(lab3 main.cpp)
target_link_libraries(${PROJECT_NAME}  ${libUtils_LIB} ${libOracle_LIB} ${libEarley_LIB} ${libLStar_LIB} ${libDFA_LIB} ${libProgramOp_LIB})
message(b = ${libOracle_LIB})
target_include_directories(lab3 PUBLIC  ${libUtils_INCLUDE_DIRS})
