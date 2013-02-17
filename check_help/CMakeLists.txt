project(check_help)
add_executable(check_help check_help.cpp)
target_link_libraries(check_help ${GEN_LIBS} pnl premia)

add_custom_command(TARGET check_help POST_BUILD       
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        $<TARGET_FILE:check_help>    
        "${OUTPUT_DIR}/$<TARGET_FILE_NAME:check_help>")
