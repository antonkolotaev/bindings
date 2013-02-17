set(OUTPUT_DIR ${PROJECT_BINARY_DIR}/_output)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR})
set(TMP_DIR ${PROJECT_BINARY_DIR}/_tmp)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_DIR})

find_package(Boost COMPONENTS filesystem python program_options system)
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")    
IF (Boost_FOUND)
     INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
     ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()    


include_directories(
	${PREMIA_DIRS_TO_INCLUDE}
	${PROJECT_SOURCE_DIR}/bindings/include
)

if (MSVC)
  set(GEN_LIBS boost_filesystem boost_program_options pnl premia)
else ()
  set(GEN_LIBS boost_filesystem boost_program_options boost_system pnl premia)
endif()


if (MSVC)
   add_subdirectory (_external)
   add_subdirectory (excel)
   add_subdirectory (python)
   add_subdirectory (karrigell)
   add_subdirectory (check_help)
endif()
