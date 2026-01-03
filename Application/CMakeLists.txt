#递归将本文件夹下所有cpp放到变量wrapper中
file(GLOB_RECURSE App ./ "*.cpp")

#编译为APP这个库
add_library(App ${App})
