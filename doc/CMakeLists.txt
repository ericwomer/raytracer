## Move to doc/CMakeListst.txt
# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(TOP_SRCDIR "${CMAKE_SOURCE_DIR}/src")
    set(TOP_BUILDDIR "${CMAKE_BINARY_DIR}")
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Documents")
    set(README_PATH "${CMAKE_SOURCE_DIR}/README.md")
    set(VERSION "${CMAKE_PROJECT_VERSION}")
    set(PROJECT_NAME "${CMAKE_PROJECT_NAME}")

    # set input and output files
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)