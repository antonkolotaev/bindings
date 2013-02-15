include_directories(
	${PROJECT_SOURCE_DIR}/bindings/include
	${PROJECT_SOURCE_DIR}/Src
	${PROJECT_SOURCE_DIR}/Src/common
	${PROJECT_SOURCE_DIR}/Src/libpnl/src/include
)
add_executable(check_help check_help.cpp)
target_link_libraries(check_help boost_filesystem boost_program_options boost_system pnl premia)

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})
add_library(pypremia SHARED pypremia.cpp)
target_link_libraries(pypremia pnl premia boost_python ${PYTHON_LIBRARIES})
set_target_properties(pypremia PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bindings/python/premia)
if (MSVC)
	set_target_properties(pypremia PROPERTIES SUFFIX ".pyd")
	set(pypremia_suffix ".pyd")
endif()
set_target_properties(pypremia PROPERTIES PREFIX "lib")
add_custom_command(TARGET pypremia POST_BUILD       
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        $<TARGET_FILE:pypremia>    
        "${CMAKE_BINARY_DIR}/bindings/python/premia/$<TARGET_FILE_NAME:pypremia>")

add_executable(pygen pygen.cpp)
target_link_libraries(pygen boost_filesystem boost_program_options boost_system pnl premia)
add_dependencies(pygen pypremia)

add_custom_command(TARGET pygen POST_BUILD       
    COMMAND $<TARGET_FILE:pygen> --output-dir="${CMAKE_BINARY_DIR}/bindings/python/premia" --dll-dir="${CMAKE_BINARY_DIR}/bindings/python/premia" --data-dir="${CMAKE_SOURCE_DIR}/data" --template-dir="${CMAKE_CURRENT_SOURCE_DIR}/templates")