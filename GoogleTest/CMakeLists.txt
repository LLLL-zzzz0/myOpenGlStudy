cmake_minimum_required(VERSION 3.12)


# 测试可执行程序
add_executable(EngineTests
    transform_test.cpp
 light_test.cpp )

# 链接你的引擎
target_link_libraries(EngineTests
    PRIVATE
    gtest
    gtest_main
    wrapper
    App
    Frame
)

enable_testing()
add_test(NAME EngineTests COMMAND EngineTests)
