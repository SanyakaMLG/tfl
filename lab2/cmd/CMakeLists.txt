set(PROJECT_NAME "lab")

project(${PROJECT_NAME})
add_executable(lab2 main.cpp)
target_link_libraries(${PROJECT_NAME}  ${libDFA_LIB} ${libBrzozowski_LIB} ${libNode_LIB} ${libDerivative_LIB} ${libRegexGenerator_LIB} ${libProgramOp_LIB})
message(b = ${libRegexGenerator_LIB})
target_include_directories(lab2 PUBLIC  ${libDFA_INCLUDE_DIRS} ${libBrzozowski_INCLUDE_DIRS} ${libNode_INCLUDE_DIRS} ${libDerivative_INCLUDE_DIRS} ${libRegexGenerator_INCLUDE_DIRS} ${libProgramOp_INCLUDE_DIRS})
