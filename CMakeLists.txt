#cmake版本
cmake_minimum_required(VERSION 3.12)

#工程名称
project(OPENGL_START)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

file(GLOB ASSETS "./assets")

file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#向项目中添加宏定义
add_definitions (-DDEBUG)

#包含头文件目录
include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/include
)

#链接库文件目录
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/lib
)

add_subdirectory(wrapper)
add_subdirectory(Application)
add_subdirectory(glframework)
add_subdirectory(imgui)
add_subdirectory(GoogleTest)


#本工程的所有cpp文件编译链接，生成exe
add_executable(openGL_study "main.cpp" "glad.c" "imgui/imconfig.h" "imgui/imgui.cpp" "imgui/imgui.h" "imgui/imgui_draw.cpp" "imgui/imgui_impl_glfw.cpp" "imgui/imgui_impl_glfw.h" "imgui/imgui_impl_opengl3.cpp" "imgui/imgui_impl_opengl3.h" "imgui/imgui_impl_opengl3_loader.h" "imgui/imgui_internal.h" "imgui/imgui_tables.cpp" "imgui/imgui_widgets.cpp" "imgui/imstb_rectpack.h" "imgui/imstb_textedit.h" "imgui/imstb_truetype.h")

#链接所需的库文件
target_link_libraries(openGL_study glfw3.lib wrapper App Frame imguilib assimp-vc143-mtd.lib zlibstaticd.lib)