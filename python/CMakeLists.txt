project(python)

set(OUTPUT_DIR ${OUTPUT_DIR}/python)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/interop.py.cmake 
    ${OUTPUT_DIR}/premia/interop.py)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/common.py
    ${OUTPUT_DIR}/premia/common.py)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/time_all_methods.py
    ${OUTPUT_DIR}/time_all_methods.py)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/get_all_method_results.py
    ${OUTPUT_DIR}/get_all_method_results.py)


find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})
add_library(pypremia SHARED pypremia.cpp)
target_link_libraries(pypremia pnl premia boost_python ${PYTHON_LIBRARIES})
if (MSVC)
	set_target_properties(pypremia PROPERTIES SUFFIX ".pyd")
	set(pypremia_suffix ".pyd")
endif()
set_target_properties(pypremia PROPERTIES PREFIX "lib")
add_custom_command(TARGET pypremia POST_BUILD       
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        $<TARGET_FILE:pypremia>    
        "${OUTPUT_DIR}/premia/$<TARGET_FILE_NAME:pypremia>")

add_executable(pygen pygen.cpp)
target_link_libraries(pygen ${GEN_LIBS})
add_dependencies(pygen pypremia)

add_custom_command(TARGET pygen POST_BUILD       
    COMMAND $<TARGET_FILE:pygen> --output-dir="${OUTPUT_DIR}/premia" --dll-dir="${OUTPUT_DIR}/premia" --data-dir="${PREMIA_DATA_DIR}" --template-dir="${CMAKE_CURRENT_SOURCE_DIR}")

