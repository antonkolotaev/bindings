project(karrigell)

add_executable(ksgen ksgen.cpp)
target_link_libraries(ksgen ${LIBS} ${GEN_LIBS} ${PYTHON_LIBRARIES})
add_dependencies(ksgen pygen)

if (KARRIGELL_INSTALLATION_PATH)
   add_custom_command(TARGET ksgen POST_BUILD       
       COMMAND $<TARGET_FILE:ksgen> --output-dir="${KARRIGELL_INSTALLATION_PATH}" --pypremia-dir="${OUTPUT_DIR}/python" --data-dir="${PREMIA_DATA_DIR}/data" --template-dir="${CMAKE_CURRENT_SOURCE_DIR}/templates")
endif (KARRIGELL_INSTALLATION_PATH)