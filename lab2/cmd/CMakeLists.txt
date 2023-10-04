set(PROJECT_NAME "lab2")

project(${PROJECT_NAME})
add_executable(lab2 main.cpp)
target_link_libraries(${PROJECT_NAME}  ${libBrzozowski_LIB} ${libNode_LIB} ${libDerivative_LIB} ${libRegexGenerator_LIB})
message(b = ${libRegexGenerator_LIB})
target_include_directories(lab2 PUBLIC  ${libBrzozowski_INCLUDE_DIRS} ${libNode_INCLUDE_DIRS} ${libDerivative_INCLUDE_DIRS} ${libRegexGenerator_INCLUDE_DIRS})
