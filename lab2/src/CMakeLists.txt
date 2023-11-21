set(PROJECT_NAME "src")
project(${PROJECT_NAME})


add_subdirectory(Node)
add_subdirectory(Derivative)
add_subdirectory(Brzozowski)
add_subdirectory(RegexGenerator)
add_subdirectory(DFA)
add_subdirectory(ProgramOp)
set(libNode_LIB ${libNode_LIB} PARENT_SCOPE)
set(libNode_INCLUDE_DIRS ${libNode_INCLUDE_DIRS} PARENT_SCOPE)

set(libDerivative_LIB ${libDerivative_LIB} PARENT_SCOPE)
set(libDerivative_INCLUDE_DIRS ${libDerivative_INCLUDE_DIRS} PARENT_SCOPE)

set(libBrzozowski_LIB ${libBrzozowski_LIB} PARENT_SCOPE)
set(libBrzozowski_INCLUDE_DIRS ${libBrzozowski_INCLUDE_DIRS} PARENT_SCOPE)

set(libRegexGenerator_LIB ${libRegexGenerator_LIB} PARENT_SCOPE)
set(libRegexGenerator_INCLUDE_DIRS ${libRegexGenerator_INCLUDE_DIRS} PARENT_SCOPE)

set(libDFA_LIB ${libDFA_LIB} PARENT_SCOPE)
set(libDFA_INCLUDE_DIRS ${libDFA_INCLUDE_DIRS} PARENT_SCOPE)

set(libProgramOp_LIB ${libProgramOp_LIB} PARENT_SCOPE)
set(libProgramOp_INCLUDE_DIRS ${libProgramOp_INCLUDE_DIRS} PARENT_SCOPE)

