set(OUTPUT_DIR ${PROJECT_BINARY_DIR}/_output)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR})
set(TMP_DIR ${PROJECT_BINARY_DIR}/_tmp)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_DIR})

if (NOT MSVC)
    find_package(Boost COMPONENTS filesystem program_options system python)
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")    
    if (Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIR})
        add_definitions( "-DHAS_BOOST" )
    endif()    
endif()

include_directories(
	${PREMIA_DIRS_TO_INCLUDE}
	${PROJECT_SOURCE_DIR}/bindings/include
    )

set(GEN_LIBS  premia ${LINK_PNL})
if (MSVC)
    set(GEN_LIBS ${GEN_LIBS} boost_filesystem boost_program_options boost_system)
else (MSVC)
    set(GEN_LIBS ${GEN_LIBS} ${Boost_LIBRARIES})
endif(MSVC)

add_subdirectory (_external)

if (MSVC)
   add_subdirectory (excel)
endif()

add_subdirectory (python)
add_subdirectory (karrigell)
add_subdirectory (check_help)
