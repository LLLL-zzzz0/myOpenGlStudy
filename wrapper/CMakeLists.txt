#递归将本文件夹下所有cpp放到变量wrapper中
file(GLOB_RECURSE wrapper ./ "*.cpp")

#编译为wrapper这个库
add_library(wrapper ${wrapper})
