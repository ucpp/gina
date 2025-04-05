project(gina_tests)

set(TEST_SOURCES
    gina_actions_tests.cpp  
    gina_math_tests.cpp  
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
    gina  
    gtest  
    gtest_main  
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${EXTERNALS_PATH}/googletest/include/
    ${CMAKE_SOURCE_DIR}/engine/public/
)

enable_testing()
add_test(NAME gina_tests COMMAND ${PROJECT_NAME})