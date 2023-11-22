project(lab3)


add_subdirectory(Earley)
add_subdirectory(utils)
add_subdirectory(OracleModule)

set(libEarley_LIB ${libEarley_LIB} PARENT_SCOPE)
set(libEarley_INCLUDE_DIRS ${libEarley_INCLUDE_DIRS} PARENT_SCOPE)

set(libUtils_LIB ${libUtils_LIB} PARENT_SCOPE)
set(libUtils_INCLUDE_DIRS ${libUtils_INCLUDE_DIRS} PARENT_SCOPE)

set(libOracle_LIB ${libOracle_LIB} PARENT_SCOPE)
set(libOracle_INCLUDE_DIRS ${libOracle_INCLUDE_DIRS} PARENT_SCOPE)

