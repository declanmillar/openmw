execute_process (
    COMMAND ${GIT_EXECUTABLE} rev-list --tags --max-count=1
    WORKING_DIRECTORY ${OpenMW_SOURCE_DIR}
    RESULT_VARIABLE EXITCODE1
    OUTPUT_VARIABLE TAGHASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET)

execute_process (
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${OpenMW_SOURCE_DIR}
    RESULT_VARIABLE EXITCODE2
    OUTPUT_VARIABLE COMMITHASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

string (COMPARE EQUAL "${EXITCODE1}:${EXITCODE2}" "0:0" FULL_SUCCESS)
string (COMPARE EQUAL "${EXITCODE2}" "0" COMMIT_SUCCESS)
if (FULL_SUCCESS)
    set(OPENMW_VERSION_COMMITHASH "${COMMITHASH}")
    set(OPENMW_VERSION_TAGHASH "${TAGHASH}")
    message(STATUS "OpenMW version ${OPENMW_VERSION}")
elseif (COMMIT_SUCCESS)
    set(OPENMW_VERSION_COMMITHASH "${COMMITHASH}")
    message(STATUS "OpenMW version ${OPENMW_VERSION}")
else ()
    message(WARNING "Failed to get valid version information from Git")
endif ()

include(${MACROSFILE})

configure_resource_file(${VERSION_RESOURCE_FILE_IN} ${OpenMW_BINARY_DIR} ${VERSION_RESOURCE_FILE_RELATIVE})
configure_file("${VERSION_CPP_FILE_IN}" "${VERSION_CPP_FILE_OUT}")
