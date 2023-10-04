set(PROJECT_NAME "src")
project(${PROJECT_NAME})


add_subdirectory(Node)
add_subdirectory(Derivative)
add_subdirectory(Brzozowski)
add_subdirectory(RegexGenerator)
set(libNode_LIB ${libNode_LIB} PARENT_SCOPE)
set(libNode_INCLUDE_DIRS ${libNode_INCLUDE_DIRS} PARENT_SCOPE)

set(libDerivative_LIB ${libDerivative_LIB} PARENT_SCOPE)
set(libDerivative_INCLUDE_DIRS ${libDerivative_INCLUDE_DIRS} PARENT_SCOPE)

set(libBrzozowski_LIB ${libBrzozowski_LIB} PARENT_SCOPE)
set(libBrzozowski_INCLUDE_DIRS ${libBrzozowski_INCLUDE_DIRS} PARENT_SCOPE)

set(libRegexGenerator_LIB ${libRegexGenerator_LIB} PARENT_SCOPE)
set(libRegexGenerator_INCLUDE_DIRS ${libRegexGenerator_INCLUDE_DIRS} PARENT_SCOPE)