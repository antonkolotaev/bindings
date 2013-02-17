project(karrigell)

add_executable(ksgen ksgen.cpp)
target_link_libraries(ksgen ${GEN_LIBS})
add_dependencies(ksgen pygen)

#add_custom_command(TARGET ksgen POST_BUILD       
#    COMMAND $<TARGET_FILE:ksgen> --output-dir="c:/Karrigell-3.1.1" --pypremia-dir="${OUTPUT_DIR}/python" --data-dir="${PREMIA_DATA_DIR}/data" --template-dir="${CMAKE_CURRENT_SOURCE_DIR}/templates")
