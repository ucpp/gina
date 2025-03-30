project(gina_tests)

add_executable(${PROJECT_NAME} gina_tests.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
    gina  
    gtest  
    gtest_main  
)

set(THIRD_PARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../externals)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${EXTERNALS_PATH}/googletest/include/)

enable_testing()
add_test(NAME gina_tests COMMAND ${PROJECT_NAME})
