#递归将本文件夹下所有cpp放到变量wrapper中
file(GLOB_RECURSE Frame ./ "*.cpp")

#编译为Frame这个库
add_library(Frame ${Frame})
