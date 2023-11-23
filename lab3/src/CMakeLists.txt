project(lab3)


add_subdirectory(Earley)
add_subdirectory(utils)
add_subdirectory(OracleModule)
add_subdirectory(LStar)
add_subdirectory(DFA)

set(libEarley_LIB ${libEarley_LIB} PARENT_SCOPE)
set(libEarley_INCLUDE_DIRS ${libEarley_INCLUDE_DIRS} PARENT_SCOPE)

set(libUtils_LIB ${libUtils_LIB} PARENT_SCOPE)
set(libUtils_INCLUDE_DIRS ${libUtils_INCLUDE_DIRS} PARENT_SCOPE)

set(libOracle_LIB ${libOracle_LIB} PARENT_SCOPE)
set(libOracle_INCLUDE_DIRS ${libOracle_INCLUDE_DIRS} PARENT_SCOPE)

set(libLStar_LIB ${libLStar_LIB} PARENT_SCOPE)
set(libLStar_INCLUDE_DIRS ${libLStar_INCLUDE_DIRS} PARENT_SCOPE)

set(libDFA_LIB ${libDFA_LIB} PARENT_SCOPE)
set(libDFA_INCLUDE_DIRS ${libDFA_INCLUDE_DIRS} PARENT_SCOPE)

